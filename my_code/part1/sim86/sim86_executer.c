#include "sim86_executer.h"

typedef void (*set_reg_func_ptr)(reg_mem_t *, uint8_t, uint16_t);
typedef uint16_t (*get_reg_func_ptr)(reg_mem_t *, uint8_t);

set_reg_func_ptr reg_setters[NUM_SETTERS] =
{
    MemorySetByteRegValue, MemorySetWordRegValue
};

get_reg_func_ptr reg_getters[NUM_GETTERS] =
{
    MemoryGetByteRegValue, MemoryGetWordRegValue
};

static uint16_t GetOperandValue(reg_mem_t *reg_mem, operand_t *operand);

void ExecutorExecInst(expression_t *instruction, reg_mem_t *reg_mem)
{
    uint8_t reg_code = GetOperandValue(reg_mem, &instruction->operands[DEST]);
    uint16_t src = GetOperandValue(reg_mem, &instruction->operands[SRC]);

    reg_setters[instruction->operands[DEST].size](reg_mem, reg_code, src);
}

static uint16_t GetOperandValue(reg_mem_t *reg_mem, operand_t *operand)
{
    switch (operand->operand_type)
    {
        case REGISTER:
        {
            // TODO: change this to get value from register!
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