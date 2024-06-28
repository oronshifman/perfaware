#include "sim86_executer.h"
#include "sim86_instruction.h"

typedef void (*set_reg_func_ptr)(reg_mem_t *, u8, u16);
typedef u16 (*get_reg_func_ptr)(reg_mem_t *, u8);
typedef void (*memory_flag_setter_func_ptr)(reg_mem_t *, u8);

set_reg_func_ptr reg_setters[NUM_REG_SETTERS] =
{
    MemorySetByteRegValue, MemorySetWordRegValue
};

get_reg_func_ptr reg_getters[NUM_REG_GETTERS] =
{
    MemoryGetByteRegValue, MemoryGetWordRegValue
};

memory_flag_setter_func_ptr mem_flag_setters[NUM_FLAG_SETTERS] =
{
    MemoryFlagOff, MemoryFlagOn
};

static u16 GetOperandValue(reg_mem_t *reg_mem, operand_t *operand);
static void MovToReg(expression_t *instruction, reg_mem_t *reg_mem);
static void ExecSub(expression_t *instruction, reg_mem_t *reg_mem);
static void ExecAdd(expression_t *instruction, reg_mem_t *reg_mem);
static void ExecCmp(expression_t *instruction, reg_mem_t *reg_mem);
static void ExecJmp(expression_t *instruction, reg_mem_t *reg_mem);
static u16 DoArithmetics(expression_t *instruction, reg_mem_t *reg_mem, u8 reg_code, u16 src_value, u8 op);
static u16 GetSignBitMask(u8 size);
static u8 GetSignBitVal(u8 size, u16 op_outcome, u16 sing_bit_mask);

void ExecutorExecInst(expression_t *instruction, reg_mem_t *reg_mem)
{
    switch(instruction->operation_type)
    {
        case MOV:
        {
            MovToReg(instruction, reg_mem);
        } break;

        case SUB:
        {
            ExecSub(instruction, reg_mem);
        } break;

        case ADD:
        {
            ExecAdd(instruction, reg_mem);
        } break;

        case CMP:
        {
            ExecCmp(instruction, reg_mem);
        } break;

        case JMP:
        {
            ExecJmp(instruction, reg_mem);
        } break;
    }
}

static void MovToReg(expression_t *instruction, reg_mem_t *reg_mem)
{
    u8 reg_code = GetOperandValue(reg_mem, &instruction->operands[DEST]);
    u16 src_value = GetOperandValue(reg_mem, &instruction->operands[SRC]);

    reg_setters[instruction->operands[DEST].size](reg_mem, reg_code, src_value);
}

static void ExecSub(expression_t *instruction, reg_mem_t *reg_mem)
{
    u8 reg_code = GetOperandValue(reg_mem, &instruction->operands[DEST]);
    u16 src_value = GetOperandValue(reg_mem, &instruction->operands[SRC]);

    u16 op_outcome = DoArithmetics(instruction, reg_mem, reg_code, src_value, SUB);

    reg_setters[instruction->operands[DEST].size](reg_mem, reg_code, op_outcome);
}

static void ExecAdd(expression_t *instruction, reg_mem_t *reg_mem)
{
    u8 reg_code = GetOperandValue(reg_mem, &instruction->operands[DEST]);
    u16 src_value = GetOperandValue(reg_mem, &instruction->operands[SRC]);

    u16 op_outcome = DoArithmetics(instruction, reg_mem, reg_code, src_value, ADD);

    reg_setters[instruction->operands[DEST].size](reg_mem, reg_code, op_outcome);
}

static void ExecCmp(expression_t *instruction, reg_mem_t *reg_mem)
{
    u8 reg_code = GetOperandValue(reg_mem, &instruction->operands[DEST]);
    u16 src_value = GetOperandValue(reg_mem, &instruction->operands[SRC]);

    u16 op_outcome = DoArithmetics(instruction, reg_mem, reg_code, src_value, SUB);
}

static void ExecJmp(expression_t *instruction, reg_mem_t *reg_mem)
{
    u8 reg_code = GetOperandValue(reg_mem, &instruction->operands[DEST]);
    u16 src_value = GetOperandValue(reg_mem, &instruction->operands[SRC]);

    switch (reg_code)
    {
        case JNE:
        {
            .....
        } break;
    }
}

static u16 DoArithmetics(expression_t *instruction, reg_mem_t *reg_mem, u8 reg_code, u16 src_value, u8 op)
{
    u16 reg_value = reg_getters[instruction->operands[DEST].size](reg_mem, reg_code);

    u16 sing_bit_mask = GetSignBitMask(instruction->operands[DEST].size);

    u16 op_outcome;
    switch (op)
    {
        case ADD:
        {
            op_outcome = reg_value + src_value;
        } break;
        
        case SUB:
        {
            op_outcome = reg_value - src_value;
        } break;

        default:
        {
            return -1;
        } break;
    }
    u8 sign_bit_val = GetSignBitVal(instruction->operands[DEST].size, op_outcome, sing_bit_mask);

    mem_flag_setters[sign_bit_val](reg_mem, SF);
    mem_flag_setters[op_outcome == 0](reg_mem, ZF);

    return op_outcome;
}

static u16 GetSignBitMask(u8 size)
{
    return size == 0 ? (1 << 7) : (1 << 15);
}

static u8 GetSignBitVal(u8 size, u16 op_outcome, u16 sing_bit_mask)
{
    u8 num_shifts = size == 0 ? 7 : 15;
    return (op_outcome & sing_bit_mask) >> num_shifts;
}

static u16 GetOperandValue(reg_mem_t *reg_mem, operand_t *operand)
{
    switch (operand->operand_type)
    {
        case REGISTER:
        {
            if (operand->direction == DEST)
            {
                return operand->reg_code;
            }
            return reg_getters[operand->size](reg_mem, operand->reg_code);
        } break;

        case EFFECTIVE_ADDR:
        {
            return operand->ea_code;
        } break;

        case DIRECT_ADDR:
        {
            return operand->disp;
        } break;

        case IMMEDIATE:
        {
            return operand->unsigned_immediate;
        } break;

        case JUMP_CODE:
        {
            return operand->jmp_code;
        } break;
    
        case JUMP_OFFSET:
        {
            return operand->jmp_offset;
        } break;

        default:
        {
            // TODO: find a default behavior
        } break;
    }
}