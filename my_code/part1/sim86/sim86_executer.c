#include "sim86_executer.h"
#include "sim86_memory.h"

reg_mem_t *reg_mem;

typedef void (*set_reg_func_ptr)(reg_mem_t *, uint8_t, uint16_t);

set_reg_func_ptr reg_setters[NUM_SETTERS] =
{
    SetByteRegValue, SetWordRegValue
};

static uint16_t GetOperandValue(operand_t *operand);

void ExecuteInstruction(expression_t *instruction)
{
    if (reg_mem == NULL)
    {
        reg_mem = InitMemory();
    }
    
    uint16_t dest = GetOperandValue(&instruction->dest);
    uint16_t src = GetOperandValue(&instruction->src);
   
    reg_setters[instruction->dest.size](reg_mem, dest, src);
}

static uint16_t GetOperandValue(operand_t *operand)
{
    switch (operand->operand_type)
    {
        case REGISTER:
        {
            return operand->reg_code;
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

void PrintMemory()
{
    PrintRegistersState(reg_mem);
}