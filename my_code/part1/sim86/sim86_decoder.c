#include "sim86_decoder.h"


#define OP_MASK  0xfc

static inst_t op_table[256] = {NONE};

static void GetInstructionForm(uint8_t op_code, inst_t *instruction);
static void InitInstructionValues(inst_t *instruction, uint16_t instruction_bytes, FILE *bin);
static void InitDecodedInst(expresion_t *decoded_inst, inst_t *full_inst, uint16_t instruction_bytes);
static void InitField(inst_t * instruction, uint8_t field, uint16_t instruction_bytes);
static void InitFieldWithExtraBytes(inst_t *instruction, uint8_t field, uint8_t bytes_to_read, FILE *bin);
static void SetDispState(inst_t *instruction);
static uint8_t isArithmeticImmediateToAcc(uint16_t instruction_bytes);
static void GetSrc(expresion_t *decoded_inst, inst_t *instruction);
static void GetDest(expresion_t *decoded_inst, inst_t *instruction, uint16_t instruction_bytes);

/**
 * 
 * @description - fills <instruction> with the next instruction decoded from <bin>
 * @return - 0 if failed 1 if success
*/
int GetNextInstruction(expresion_t *decoded_inst, FILE *bin)
{
    if (op_table[0].operation_type == NONE)
    {
        InitOpTable(op_table);
    }

    uint16_t instruction_bytes = 0;
    if(fread(&instruction_bytes, sizeof(instruction_bytes), 1, bin) != 1) 
    {
        if (ferror(bin))
        {
            perror("GetNextInstruction");
        }
        return 0;
    }

    inst_t full_inst;
    GetInstructionForm(instruction_bytes & OP_MASK, &full_inst);

    InitInstructionValues(&full_inst, instruction_bytes, bin);

    InitDecodedInst(decoded_inst, &full_inst, instruction_bytes);

    return 1;
}

static void GetInstructionForm(uint8_t op_code, inst_t *full_inst) 
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

static void InitDecodedInst(expresion_t *decoded_inst, inst_t *full_inst, uint16_t instruction_bytes)
{
    decoded_inst->operation_type = full_inst->operation_type;
    GetDest(decoded_inst, full_inst, instruction_bytes);
    GetSrc(decoded_inst, full_inst);
}

static void GetSrc(expresion_t *decoded_inst, inst_t *full_inst)
{
    operand_t *src = &decoded_inst->src;
    if (full_inst->field[D].value)
    {
        src->operand_type = EFFECTIVE_ADDR;
        src->size = full_inst->field[W].value;
        src->ea_code = full_inst->field[RM].value;
        if (full_inst->field[DISP].state == INITIALIZED)
        {
            src->disp = full_inst->field[DISP].value;
        }
        else
        {
            src->disp = 0;
        }
    }

    if (full_inst->field[DATA].state == INITIALIZED)
    {
        src->operand_type = IMMEDIATE;
        src->size = full_inst->field[W].value;
        src->unsigned_immediate = full_inst->field[DATA].value;
        src->disp = 0;
        return;
    }

    src->operand_type = REGISTER;
    src->size = full_inst->field[W].value;
    src->ea_code = full_inst->field[REG].value;
    src->disp = 0;
    // if (full_inst->field[D].value)
    // {
    //     decoded_inst->src = full_inst->field[RM];
    //     decoded_inst->disp = full_inst->field[DISP];
    //     return;
    // }

    // if (full_inst->field[DATA].state == INITIALIZED)
    // {
    //     decoded_inst->src = full_inst->field[DATA];
    //     return;
    // }

    // decoded_inst->src = full_inst->field[REG];
}

static void GetDest(expresion_t *decoded_inst, inst_t *full_inst, uint16_t instruction_bytes)
{
    if (full_inst->operation_type == JMP)
    {
        decoded_inst->dest = full_inst->field[JMP_OFFSET];
        decoded_inst->dest.mask = instruction_bytes >> full_inst->field[JMP_OFFSET].offset; // NOTE: the op_code for the jump is in the mask field
        return;
    }

    // NOTE: if mod is NOT used this is "immediate to register" or "mem to accumulator" (true for arithmetics too)
    if (full_inst->field[MOD].state == NOT_USED)
    {
        decoded_inst->dest = full_inst->field[REG];
        return;
    }

    // NOTE: if mod IS used this is "immediate to rm", "rm to/from reg" (true for arithmetics too)
    if (full_inst->field[D].state == INITIALIZED && full_inst->field[D].value)
    {
        decoded_inst->dest = full_inst->field[REG];
        return;
    }

    if (full_inst->field[MOD].value == 0b11)
    {
        decoded_inst->dest = full_inst->field[RM];
        decoded_inst->dest.field_type = REG;
        return;
    }

    if (full_inst->field[MOD].value == 0b00 && full_inst->field[RM].value == 0b110)
    {
        decoded_inst->dest = full_inst->field[DISP];
        return;
    }
    
    decoded_inst->dest = full_inst->field[RM];
    decoded_inst->disp = full_inst->field[DISP];
}

static void InitInstructionValues(inst_t *instruction, uint16_t instruction_bytes, FILE *bin)
{
    enum operation_type inst_operation_type = instruction->operation_type;
    if (inst_operation_type == NONE)
    {
        uint8_t imdt_to_reg[] = {ADD,NONE,NONE,NONE,NONE,SUB,NONE,CMP};
        
        uint8_t field_offset = instruction->field[REG].offset;
        uint16_t field_mask = instruction->field[REG].mask << field_offset;

        instruction->operation_type = imdt_to_reg[(instruction_bytes & field_mask) >> field_offset];
    }

    for (uint8_t field_type = 0; field_type < DISP; ++field_type) 
    {
        if (instruction->field[field_type].state == UNINITIALIZED)
        {
            InitField(instruction, field_type, instruction_bytes);
        }
    }

    SetDispState(instruction);

    if (instruction->field[DISP].state == UNINITIALIZED)
    {
        uint8_t disp_size = instruction->field[MOD].value == 0 ? 
                            instruction->field[DISP].mask :
                            instruction->field[MOD].value;
        InitFieldWithExtraBytes(instruction,
                                DISP,
                                disp_size, 
                                bin);
    }

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
                uint8_t extra_byte = 0;
                fread(&extra_byte, instruction->field[W].value, 1, bin);
                
                uint8_t offset = instruction->field[DATA].offset;
                uint16_t mask = instruction->field[DATA].mask << offset;

                instruction->field[DATA].value = (instruction_bytes & mask) >> offset | extra_byte << offset; 
                instruction->field[DATA].state = INITIALIZED;
            }
            return;
        }

        uint8_t w = instruction->field[W].value;
        uint8_t s = instruction->field[S].value;
        uint8_t bytes_to_read = 0;
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
                                bin);
    }
}

static uint8_t isArithmeticImmediateToAcc(uint16_t instruction_bytes) {
    uint8_t ADD_IMMEDIATE_TO_ACC = 0b00000100;
    uint8_t SUB_IMMEDIATE_TO_ACC = 0b00000100;
    uint8_t CMP_IMMEDIATE_TO_ACC = 0b00000100;

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

static void InitField(inst_t *instruction, uint8_t field, uint16_t instruction_bytes)
{
    uint8_t field_offset = instruction->field[field].offset;
    uint16_t field_mask = instruction->field[field].mask << field_offset;

    instruction->field[field].value = (instruction_bytes & field_mask) >> field_offset;
    instruction->field[field].state = INITIALIZED;
}

static void InitFieldWithExtraBytes(inst_t *instruction, uint8_t field, uint8_t bytes_to_read, FILE *bin)
{
    fread(&instruction->field[field].value, bytes_to_read, 1, bin);
    instruction->field[field].state = INITIALIZED;
}

static void SetDispState(inst_t *instruction)
{
    if (instruction->field[MOD].value == 0b01 || 
        instruction->field[MOD].value == 0b10)
    {
        instruction->field[DISP].state = UNINITIALIZED;
        return;
    }
    else if (instruction->field[MOD].value == 0b00 && 
             instruction->field[RM].value == 0b110)
    {
        instruction->field[DISP].state = UNINITIALIZED;
        instruction->field[DISP].mask = 2; // NOTE: the size to read is in the mask
        return;
    }
    instruction->field[DISP].state = NOT_USED;
}
