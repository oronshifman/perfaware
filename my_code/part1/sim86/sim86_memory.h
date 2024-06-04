#ifndef __SIM86_MEMORY_H__
#define __SIM86_MEMORY_H__

#include <stdint.h>

#define NUM_REGS 8

#define NUM_SETTERS 2
#define NUM_GETTERS 2

enum reg_types
{
    BYTE,
    WORD,

    REG_TYPES
};

enum w_reg
{
    AX,
    BX = 2,
    CX = 4,
    DX = 6,
    SP = 8,
    BP = 10,
    SI = 12,
    DI = 14,

    WORD_REGS
};


enum b_reg
{
    AL, AH,
    BL, BH,
    CL, CH,
    DL, DH,

    BYTE_REGS
};

typedef struct reg_mem
{
    uint8_t memory[REG_TYPES * NUM_REGS];
} reg_mem_t;

reg_mem_t *InitMemory(void);
void SetWordRegValue(reg_mem_t *registers, uint8_t reg, uint16_t value);
void SetByteRegValue(reg_mem_t *registers, uint8_t reg, uint16_t value);
uint16_t GetWordRegValue(reg_mem_t *registers, uint8_t reg);
uint8_t GetByteRegValue(reg_mem_t *registers, uint8_t reg);
void PrintRegistersState(reg_mem_t *registers);

#endif /* __SIM86_MEMORY_H__ */ 

// compile only memory_test
// gcc -I. sim86_memory_test.c sim86_memory.c -o test -g

/*
MY_ENCODE  SPEC_ENCODE
ax = 0     (000) 
al = 0     (000) 
ah = 1     (100)
   
bx = 2     (011) 
bl = 2     (011) 
bh = 3     (111)
   
cx = 4     (001) 
cl = 4     (001) 
ch = 5     (101)
   
dx = 6     (010) 
dl = 6     (010) 
dh = 7     (110)
   
sp = 8     (100)
bp = 10    (101)
si = 12    (110)
di = 14    (111)
*/