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

reg_mem_t *InitMemory(void)
{
    reg_mem_t *registers = (reg_mem_t *)calloc(1, sizeof(*registers));
    if (!registers)
    {
        perror("Failed to init registers");
        exit(1);
    }

    return registers;
}

void SetWordRegValue(reg_mem_t *registers, uint8_t reg, uint16_t value)
{
    assert(registers);
    assert(reg < WORD_REGS);

    uint8_t reg_trans = translation_table[WORD][reg];
    uint16_t *where = (uint16_t *)&(registers->memory[reg_trans]);
    *where = value;
}

void SetByteRegValue(reg_mem_t *registers, uint8_t reg, uint16_t value)
{
    assert(registers);
    assert(reg < BYTE_REGS);

    uint8_t reg_trans = translation_table[BYTE][reg];
    registers->memory[reg_trans] = value;
}

uint16_t GetWordRegValue(reg_mem_t *registers, uint8_t reg)
{
    assert(registers);
    assert(reg < WORD_REGS);

    return *(uint16_t *)&(registers->memory[reg]);
}

uint8_t GetByteRegValue(reg_mem_t *registers, uint8_t reg)
{
    assert(registers);
    assert(reg < BYTE_REGS);

    return registers->memory[reg];
}

void PrintRegistersState(reg_mem_t *registers)
{
    printf("ax - 0x%x\n", *(uint16_t *)(registers->memory + AX));
    printf("bx - 0x%x\n", *(uint16_t *)(registers->memory + BX));
    printf("cx - 0x%x\n", *(uint16_t *)(registers->memory + CX));
    printf("dx - 0x%x\n", *(uint16_t *)(registers->memory + DX));
    printf("sp - 0x%x\n", *(uint16_t *)(registers->memory + SP));
    printf("bp - 0x%x\n", *(uint16_t *)(registers->memory + BP));
    printf("si - 0x%x\n", *(uint16_t *)(registers->memory + SI));
    printf("di - 0x%x\n\n", *(uint16_t *)(registers->memory + DI));

    printf("al - 0x%x\n", registers->memory[AL]);
    printf("ah - 0x%x\n", registers->memory[AH]);
    printf("bl - 0x%x\n", registers->memory[BL]);
    printf("bh - 0x%x\n", registers->memory[BH]);
    printf("cl - 0x%x\n", registers->memory[CL]);
    printf("ch - 0x%x\n", registers->memory[CH]);
    printf("dl - 0x%x\n", registers->memory[DL]);
    printf("dh - 0x%x\n", registers->memory[DH]);
}
