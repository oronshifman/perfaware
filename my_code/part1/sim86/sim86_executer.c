#include "sim86_executer.h"

typedef void (*set_reg_func_ptr)(reg_mem_t *, uint8_t, uint16_t);

set_reg_func_ptr reg_setters[NUM_SETTERS] =
{
    SetByteRegValue, SetWordRegValue
};

static uint16_t GetOperandValue(operand_t *operand);

void ExecuteInstruction(expression_t *instruction, reg_mem_t *reg_mem)
{
    uint8_t reg_code = GetOperandValue(&instruction->dest);
    uint16_t src = GetOperandValue(&instruction->src);

    reg_setters[instruction->dest.size](reg_mem, reg_code, src);
}

static uint16_t GetOperandValue(operand_t *operand)
{
    switch (operand->operand_type)
    {
        case REGISTER:
        {
            return operand->reg_code; // TODO: change this to get value from register!
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