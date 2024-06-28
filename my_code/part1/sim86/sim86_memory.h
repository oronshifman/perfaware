#ifndef __SIM86_MEMORY_H__
#define __SIM86_MEMORY_H__

#include "my_int.h"

#define BYTES_IN_KB 1024
#define KB_IN_MB 1024
#define NUM_GENERAL_PURPOSE_REG 8

#define NUM_REG_SETTERS 2
#define NUM_REG_GETTERS 2
#define NUM_FLAG_SETTERS 2

enum reg_types
{
    BYTE,
    WORD,
    SEGMENT,
    
    REG_TYPES
};

enum befor_after_exec
{
    BEFOR_EXEC,
    AFTER_EXEC
};

enum segments
{
    CODE_SEG,
    DATA_SEG,
    STACK_SEG,
    EXTRA_SEG,

    NUM_SEGMENTS
};

enum flags
{
    ZF,
    SF,

    NUM_FLAGS
};

typedef struct reg_mem reg_mem_t;

reg_mem_t *MemoryCreate(void);
void MemoryDestroy(reg_mem_t *reg_mem);

void MemorySetWordRegValue(reg_mem_t *reg_mem, u8 reg, u16 value);
void MemorySetByteRegValue(reg_mem_t *reg_mem, u8 reg, u16 value);
u16 MemoryGetWordRegValue(reg_mem_t *reg_mem, u8 reg);
u16 MemoryGetByteRegValue(reg_mem_t *reg_mem, u8 reg);

void MemoryPrintAllReg(reg_mem_t *reg_mem);
void MemoryPrintSingleReg(reg_mem_t *reg_mem, u8 size, u8 reg, enum befor_after_exec when);
void MemoryPrintFlags(reg_mem_t *reg_mem);
void MemoryPrintIPReg(reg_mem_t *reg_mem,  enum befor_after_exec when);

void MemoryFlagOn(reg_mem_t *reg_mem, u8 flag);
void MemoryFlagOff(reg_mem_t *reg_mem, u8 flag);
u8 MemoryGetFlag(reg_mem_t *reg_mem, u8 flag);

/**
 * 
 * @param n - Can only be ether 1 or 2
 * @return - Always 2 bytes from reg_mem->memory[segment + offset] and increments IP by n
*/
u16 MemoryGetNextNByteMemory(reg_mem_t *reg_mem, u8 segment, u8 n);
u16 MemoryGetIP(reg_mem_t *reg_mem);
void MemoryChangeIPByN(reg_mem_t *reg_mem, u16 n);
s64 MemorySetupCodeSeg(reg_mem_t *reg_mem, FILE *bin);

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