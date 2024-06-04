#include "sim86_executer.h"
#include "sim86_memory.h"

reg_mem_t *reg_mem;

typedef void (*set_reg_func_ptr)(reg_mem_t *, uint8_t, uint16_t);

set_reg_func_ptr reg_setters[NUM_SETTERS] =
{
    SetByteRegValue, SetWordRegValue
};

void ExecuteInstruction(expresion_t *instruction)
{
    if (reg_mem == NULL)
    {
        reg_mem = InitMemory();
    }
    
    // reg_setters[instruction->field[W].value](reg_mem, dest, src);
}

static void ToRm(inst_t *instruction, uint8_t dest, uint16_t src)
{
}

void PrintMemory()
{
    PrintRegistersState(reg_mem);
}