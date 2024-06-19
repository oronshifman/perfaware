#include "sim86_executer.h"

typedef void (*set_reg_func_ptr)(reg_mem_t *, uint8_t, uint16_t);
typedef uint16_t (*get_reg_func_ptr)(reg_mem_t *, uint8_t);
typedef void (*memory_flag_setter_func_ptr)(reg_mem_t *, uint8_t);

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

static uint16_t GetOperandValue(reg_mem_t *reg_mem, operand_t *operand);
static void MovToReg(expression_t *instruction, reg_mem_t *reg_mem);
static void ExecSub(expression_t *instruction, reg_mem_t *reg_mem);
static void ExecAdd(expression_t *instruction, reg_mem_t *reg_mem);
static void ExecCmp(expression_t *instruction, reg_mem_t *reg_mem);
static void ExecJmp(expression_t *instruction, reg_mem_t *reg_mem);
static uint16_t GetSignBitMask(uint8_t size);
static uint8_t GetSignBitVal(uint8_t size, uint16_t op_outcome, uint16_t sing_bit_mask);

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
    uint8_t reg_code = GetOperandValue(reg_mem, &instruction->operands[DEST]);
    uint16_t src_value = GetOperandValue(reg_mem, &instruction->operands[SRC]);

    reg_setters[instruction->operands[DEST].size](reg_mem, reg_code, src_value);
}

static void ExecSub(expression_t *instruction, reg_mem_t *reg_mem)
{
    uint8_t reg_code = GetOperandValue(reg_mem, &instruction->operands[DEST]);
    uint16_t src_value = GetOperandValue(reg_mem, &instruction->operands[SRC]);

    uint16_t reg_value = reg_getters[instruction->operands[DEST].size](reg_mem, reg_code);

    uint16_t sing_bit_mask = GetSignBitMask(instruction->operands[DEST].size);

    uint16_t op_outcome = reg_value - src_value;
    uint8_t sign_bit_val = GetSignBitVal(instruction->operands[DEST].size, op_outcome, sing_bit_mask);

    reg_setters[instruction->operands[DEST].size](reg_mem, reg_code, op_outcome);
    mem_flag_setters[sign_bit_val](reg_mem, SF);
    mem_flag_setters[op_outcome == 0](reg_mem, ZF);

    // NOTE: for debug and feedback
    // printf("ZF: %d, SF: %d\n", MemoryGetFlag(reg_mem, ZF), MemoryGetFlag(reg_mem, SF));
}

static void ExecAdd(expression_t *instruction, reg_mem_t *reg_mem)
{
}

static void ExecCmp(expression_t *instruction, reg_mem_t *reg_mem)
{
    uint8_t reg_code = GetOperandValue(reg_mem, &instruction->operands[DEST]);
    uint16_t src_value = GetOperandValue(reg_mem, &instruction->operands[SRC]);

    uint16_t reg_value = reg_getters[instruction->operands[DEST].size](reg_mem, reg_code);
    
}

static void ExecJmp(expression_t *instruction, reg_mem_t *reg_mem)
{
}

static uint16_t GetSignBitMask(uint8_t size)
{
    return size == 0 ? (1 << 7) : (1 << 15);
}

static uint8_t GetSignBitVal(uint8_t size, uint16_t op_outcome, uint16_t sing_bit_mask)
{
    uint8_t num_shifts = size == 0 ? 7 : 15;
    return (op_outcome & sing_bit_mask) >> num_shifts;
}

static uint16_t GetOperandValue(reg_mem_t *reg_mem, operand_t *operand)
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

        default:
        {
            // TODO: find a default behavior
        } break;
    }
}