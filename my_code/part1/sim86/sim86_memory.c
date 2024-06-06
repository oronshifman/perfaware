#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>

#include "sim86_memory.h"

uint8_t translation_table[REG_TYPES][NUM_REGS] = 
{
    {AL, CL, DL, BL, AH, CH, DH, BH},
    {AX, CX, DX, BX, SP, BP, SI, DI}
};

typedef uint16_t (*get_reg_func_ptr)(reg_mem_t *, uint8_t);

reg_mem_t *InitMemory(void)
{
    reg_mem_t *reg_mem = (reg_mem_t *)calloc(1, sizeof(*reg_mem));
    if (!reg_mem)
    {
        perror("Failed to init reg_mem");
        exit(1);
    }

    return reg_mem;
}

void SetWordRegValue(reg_mem_t *reg_mem, uint8_t reg, uint16_t value)
{
    assert(reg_mem);
    assert(reg < WORD_REGS);

    uint8_t reg_trans = translation_table[WORD][reg];
    uint16_t *where = (uint16_t *)&(reg_mem->memory[reg_trans]);
    *where = value;
}

void SetByteRegValue(reg_mem_t *reg_mem, uint8_t reg, uint16_t value)
{
    assert(reg_mem);
    assert(reg < BYTE_REGS);

    uint8_t reg_trans = translation_table[BYTE][reg];
    reg_mem->memory[reg_trans] = value;
}

uint16_t GetWordRegValue(reg_mem_t *reg_mem, uint8_t reg)
{
    assert(reg_mem);
    assert(reg < WORD_REGS);

    return *(uint16_t *)&(reg_mem->memory[reg]);
}

uint16_t GetByteRegValue(reg_mem_t *reg_mem, uint8_t reg)
{
    assert(reg_mem);
    assert(reg < BYTE_REGS);

    return reg_mem->memory[reg];
}

void PrintSingleRegister(reg_mem_t *reg_mem, uint8_t size, uint8_t reg, enum befor_after_exec when)
{
    char *reg_table[REG_TYPES][NUM_REGS] = 
    {
        // byte (0)
        {"al", "cl", "dl", "bl", 
         "ah", "ch", "dh", "bh"},
        // word (1)
        {"ax", "cx", "dx", "bx",
         "sp", "bp", "si", "di"}
    };

    get_reg_func_ptr reg_getters[NUM_GETTERS] =
    {
        GetByteRegValue, GetWordRegValue
    };
    
    uint8_t reg_trans = translation_table[size][reg];
    
    if (when == BEFOR_EXEC)
    {
        printf(" ; %s: (0x%x)->", reg_table[size][reg], reg_getters[size](reg_mem, reg_trans));
        return;
    }
    printf("(0x%x)", reg_getters[size](reg_mem, reg_trans));
}

void PrintAllRegisters(reg_mem_t *reg_mem)
{
    printf("ax - 0x%x (%d)\n", *(uint16_t *)(reg_mem->memory + AX), *(uint16_t *)(reg_mem->memory + AX));
    printf("bx - 0x%x (%d)\n", *(uint16_t *)(reg_mem->memory + BX), *(uint16_t *)(reg_mem->memory + BX));
    printf("cx - 0x%x (%d)\n", *(uint16_t *)(reg_mem->memory + CX), *(uint16_t *)(reg_mem->memory + CX));
    printf("dx - 0x%x (%d)\n", *(uint16_t *)(reg_mem->memory + DX), *(uint16_t *)(reg_mem->memory + DX));
    printf("sp - 0x%x (%d)\n", *(uint16_t *)(reg_mem->memory + SP), *(uint16_t *)(reg_mem->memory + SP));
    printf("bp - 0x%x (%d)\n", *(uint16_t *)(reg_mem->memory + BP), *(uint16_t *)(reg_mem->memory + BP));
    printf("si - 0x%x (%d)\n", *(uint16_t *)(reg_mem->memory + SI), *(uint16_t *)(reg_mem->memory + SI));
    printf("di - 0x%x (%d)\n\n", *(uint16_t *)(reg_mem->memory + DI), *(uint16_t *)(reg_mem->memory + DI));

    printf("al - 0x%x (%d)\n", reg_mem->memory[AL], reg_mem->memory[AL]);
    printf("ah - 0x%x (%d)\n", reg_mem->memory[AH], reg_mem->memory[AH]);
    printf("bl - 0x%x (%d)\n", reg_mem->memory[BL], reg_mem->memory[BL]);
    printf("bh - 0x%x (%d)\n", reg_mem->memory[BH], reg_mem->memory[BH]);
    printf("cl - 0x%x (%d)\n", reg_mem->memory[CL], reg_mem->memory[CL]);
    printf("ch - 0x%x (%d)\n", reg_mem->memory[CH], reg_mem->memory[CH]);
    printf("dl - 0x%x (%d)\n", reg_mem->memory[DL], reg_mem->memory[DL]);
    printf("dh - 0x%x (%d)\n", reg_mem->memory[DH], reg_mem->memory[DH]);
}
