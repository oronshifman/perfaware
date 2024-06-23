#include "sim86_decoder.h"

#define OP_MASK  0xfc

static inst_t op_table[256] = {NONE};

static void GetInstructionForm(u8 op_code, inst_t *instruction);
static void InitInstructionValues(inst_t *instruction, u16 instruction_bytes, reg_mem_t *reg_mem, u8 *bytes_read);
static void InitDecodedInst(expression_t *decoded_inst, inst_t *full_inst, u16 instruction_bytes);
static void InitField(inst_t * instruction, u8 field, u16 instruction_bytes);
static void InitFieldWithExtraBytes(inst_t *instruction, u8 field, u8 bytes_to_read, reg_mem_t *reg_mem);
static u8 isArithmeticImmediateToAcc(u16 instruction_bytes);
static void GetSrc(expression_t *decoded_inst, inst_t *instruction, u16 instruction_bytes);
static void GetDest(expression_t *decoded_inst, inst_t *instruction, u16 instruction_bytes);
static void InitOperand(operand_t *operand, enum operand_type type, u8 size, u16 disp);
/**
 * 
 * @description - fills <instruction> with the next instruction decoded from <bin>
 * @return - number of bytes read from code segment
*/
u8 DecoderGetNextInst(expression_t *decoded_inst, reg_mem_t *reg_mem)
{
    if (op_table[0].operation_type == NONE)
    {
        InstructionInitOpTable(op_table);
    }

    // TODO(23.7.24): changed from work with IP (3 lines)
    u16 instruction_bytes = MemoryNextGetNByteMemory(reg_mem, CODE_SEG, sizeof(instruction_bytes)); // TODO(23.7.24): under construction
    u8 bytes_read = sizeof(instruction_bytes);

    inst_t full_inst;
    GetInstructionForm(instruction_bytes & OP_MASK, &full_inst);

    InitInstructionValues(&full_inst, instruction_bytes, reg_mem, &bytes_read); // TODO(23.7.24): under construction

    InitDecodedInst(decoded_inst, &full_inst, instruction_bytes);

    return bytes_read;
}

static void GetInstructionForm(u8 op_code, inst_t *full_inst) 
{
    full_inst->operation_type = op_table[op_code].operation_type;
    full_inst->field[D] = op_table[op_code].field[D];
    full_inst->field[W] = op_table[op_code].field[W];
    full_inst->field[S] = op_table[op_code].field[S];
    full_inst->field[MOD] = op_table[op_code].field[MOD];
    full_inst->field[REG] = op_table[op_code].field[REG];
    full_inst->field[RM] = op_table[op_code].field[RM];
    full_inst->field[DISP] = op_table[op_code].field[DISP];
    full_inst->field[DATA] = op_table[op_code].field[DATA];
    full_inst->field[JMP_OFFSET] = op_table[op_code].field[JMP_OFFSET];
}

static void InitDecodedInst(expression_t *decoded_inst, inst_t *full_inst, u16 instruction_bytes)
{
    decoded_inst->operation_type = full_inst->operation_type;
    GetDest(decoded_inst, full_inst, instruction_bytes);
    GetSrc(decoded_inst, full_inst, instruction_bytes);
}

static void GetSrc(expression_t *decoded_inst, inst_t *full_inst, u16 instruction_bytes)
{
    operand_t *src = &decoded_inst->operands[SRC];
    src->direction = SRC;

    if (full_inst->operation_type == JMP)
    {
        InitOperand(src, JUMP_OFFSET, full_inst->field[W].value, 0);
        src->jmp_offset = full_inst->field[JMP_OFFSET].value;

        return;
    }

    if (full_inst->field[D].value)
    {
        src->ea_code = full_inst->field[RM].value;
        if (full_inst->field[DISP].state == INITIALIZED)
        {
            InitOperand(src, EFFECTIVE_ADDR, full_inst->field[W].value, full_inst->field[DISP].value);
        }
        else
        {
            InitOperand(src, EFFECTIVE_ADDR, full_inst->field[W].value, 0);
        }

        return;
    }

    if (full_inst->field[DATA].state == INITIALIZED)
    {
        InitOperand(src, IMMEDIATE, full_inst->field[W].value, 0);
        src->unsigned_immediate = full_inst->field[DATA].value;

        return;
    }

    InitOperand(src, REGISTER, full_inst->field[W].value, 0);
    src->ea_code = full_inst->field[REG].value;
}


static void GetDest(expression_t *decoded_inst, inst_t *full_inst, u16 instruction_bytes)
{
    operand_t *dest = &decoded_inst->operands[DEST];
    dest->direction = DEST;
    
    if (full_inst->operation_type == JMP)
    {
        InitOperand(dest, JUMP_CODE, full_inst->field[W].value, 0);

        u8 jmp_code = (instruction_bytes & 0xff) - 112 < 100 ? 
                           (instruction_bytes & 0xff) - 112 :
                           (instruction_bytes & 0xff) - 212 + 4;
        dest->jmp_code = jmp_code;

        return;
    }
    
    // NOTE: if mod is NOT used this is "immediate to register" or "mem to accumulator" (true for arithmetics too)
    if (full_inst->field[MOD].state == NOT_USED)
    {
        InitOperand(dest, REGISTER, full_inst->field[W].value, 0);
        dest->reg_code = full_inst->field[REG].value;

        return;
    }

    // NOTE: if mod IS used this is "immediate to rm", "rm to/from reg" (true for arithmetics too)
    if ((full_inst->field[D].state == INITIALIZED && full_inst->field[D].value))
    {
        InitOperand(dest, REGISTER, full_inst->field[W].value, 0);
        dest->reg_code = full_inst->field[REG].value;

        return;
    }

    if (full_inst->field[MOD].value == 0b11)
    {
        InitOperand(dest, REGISTER, full_inst->field[W].value, 0);
        dest->reg_code = full_inst->field[RM].value;

        return;
    }

    // NOTE: dest is direct address
    if (full_inst->field[MOD].value == 0b00 && full_inst->field[RM].value == 0b110)
    {
        InitOperand(dest, DIRECT_ADDR, full_inst->field[W].value, full_inst->field[DISP].value);
        dest->ea_code = full_inst->field[RM].value;

        return;
    }
    
    // NOTE: else dest is ea with disp
    InitOperand(dest, EFFECTIVE_ADDR, full_inst->field[W].value, full_inst->field[DISP].value);
    dest->ea_code = full_inst->field[RM].value;
}

static void InitOperand(operand_t *operand, enum operand_type type, u8 size, u16 disp)
{
    operand->operand_type = type;
    operand->size = size;
    operand->disp = disp;
}

static void InitInstructionValues(inst_t *instruction, u16 instruction_bytes, reg_mem_t *reg_mem, u8 *bytes_read)
{
    enum operation_type inst_operation_type = instruction->operation_type;
    if (inst_operation_type == NONE)
    {
        u8 imdt_to_reg[] = {ADD,NONE,NONE,NONE,NONE,SUB,NONE,CMP};
        
        u8 field_offset = instruction->field[REG].offset;
        u16 field_mask = instruction->field[REG].mask << field_offset;

        instruction->operation_type = imdt_to_reg[(instruction_bytes & field_mask) >> field_offset];
    }

    for (u8 field_type = 0; field_type < DISP; ++field_type) 
    {
        if (instruction->field[field_type].state == UNINITIALIZED)
        {
            InitField(instruction, field_type, instruction_bytes);
        }
    }

    // NOTE: set disp state
    if (instruction->field[MOD].value == 0b01 || 
        instruction->field[MOD].value == 0b10)
    {
        instruction->field[DISP].state = UNINITIALIZED;
    }
    else if (instruction->field[MOD].value == 0b00 && 
             instruction->field[RM].value == 0b110)
    {
        instruction->field[DISP].state = UNINITIALIZED;
        instruction->field[DISP].mask = 2; // NOTE: the size to read is in the mask
    }
    else
    {
        instruction->field[DISP].state = NOT_USED;
    }

    // NOTE: initialize disp
    if (instruction->field[DISP].state == UNINITIALIZED)
    {
        u8 disp_size = instruction->field[MOD].value == 0 ? 
                            instruction->field[DISP].mask :
                            instruction->field[MOD].value;
        InitFieldWithExtraBytes(instruction,
                                DISP,
                                disp_size, 
                                reg_mem); // TODO(23.7.24): changed from bin to reg_mem
        *bytes_read += disp_size;
    }

    // NOTE: initialize data
    if (instruction->field[DATA].state == UNINITIALIZED)
    {
        if ((instruction_bytes & OP_MASK) >> 4 == 0b1011 ||
            isArithmeticImmediateToAcc(instruction_bytes))
        {
            if (instruction->field[W].value == 0)
            {
                InitField(instruction, DATA, instruction_bytes);
            }
            else
            {
                // TODO(23.7.24): changed from work with IP (3 lines)
                u8 extra_byte = MemoryNextGetNByteMemory(reg_mem, CODE_SEG, sizeof(extra_byte));
                *bytes_read += sizeof(extra_byte);
                
                u8 offset = instruction->field[DATA].offset;
                u16 mask = instruction->field[DATA].mask << offset;

                instruction->field[DATA].value = (instruction_bytes & mask) >> offset | extra_byte << offset; 
                instruction->field[DATA].state = INITIALIZED;
            }
            return;
        }

        u8 w = instruction->field[W].value;
        u8 s = instruction->field[S].value;
        u8 bytes_to_read = 0;
        if (instruction->field[S].state != NOT_USED)
        {
            if (w == 1 && s == 1 || w == 0 && s == 0)
            {
                bytes_to_read = 1;
            }
            else if (w == 1 && s == 0)
            {
                bytes_to_read = 2;
            }
        }
        else if (instruction->operation_type != MOV)
        {
            bytes_to_read = instruction->field[W].value;
        }
        else
        {
            bytes_to_read = instruction->field[W].value == 1 ? 2 : 1;
        }

        InitFieldWithExtraBytes(instruction,
                                DATA,
                                bytes_to_read, 
                                reg_mem); // TODO(23.7.24): changed from bin to reg_mem
        *bytes_read += bytes_to_read;
    }
}

static u8 isArithmeticImmediateToAcc(u16 instruction_bytes) {
    u8 ADD_IMMEDIATE_TO_ACC = 0b00000100;
    u8 SUB_IMMEDIATE_TO_ACC = 0b00000100;
    u8 CMP_IMMEDIATE_TO_ACC = 0b00000100;

    switch (instruction_bytes & OP_MASK)
    {
        case 0b00000100: // ADD_IMMEDIATE_TO_ACC
        {
            return 1;
        } break;
        
        case 0b00101100: // SUB_IMMEDIATE_TO_ACC
        {
            return 1;
        } break;
            
        case 0b00111100: // CMP_IMMEDIATE_TO_ACC
        {
            return 1;
        } break;

        default:
        {
            return 0;
        } break;
    }
}

static void InitField(inst_t *instruction, u8 field, u16 instruction_bytes)
{
    u8 field_offset = instruction->field[field].offset;
    u16 field_mask = instruction->field[field].mask << field_offset;

    instruction->field[field].value = (instruction_bytes & field_mask) >> field_offset;
    instruction->field[field].state = INITIALIZED;
}

static void InitFieldWithExtraBytes(inst_t *instruction, u8 field, u8 bytes_to_read, reg_mem_t *reg_mem)
{
    switch (bytes_to_read)
    {
        case 1:
        {
            instruction->field[field].value = (u8) MemoryNextGetNByteMemory(reg_mem, CODE_SEG, bytes_to_read);
        } break;
        case 2:
        {
            instruction->field[field].value = MemoryNextGetNByteMemory(reg_mem, CODE_SEG, bytes_to_read);
        } break;
    }
    instruction->field[field].state = INITIALIZED;
}
