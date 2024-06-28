#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "sim86_memory.h"

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

enum special_reg
{
    CS = 16,
    DS = 18,
    SS = 20,
    ES = 22,
    IP = 24,
    FLAGS_REG = 26,

    SPECIAL_REGS = 6
};

enum flags_reg
{
    FLAGS_REG_SIZE = 1,

    SF_INDEX = 6,
    ZF_INDEX = 7,

    ZF_MASK = 0x80,
    SF_MASK = 0x40,
};

u8 flag_mask_table[NUM_FLAGS] =
{
    ZF_MASK, SF_MASK
};

u8 flag_index_table[NUM_FLAGS] =
{
    ZF_INDEX, SF_INDEX
};

u8 register_translation_table[REG_TYPES][NUM_GENERAL_PURPOSE_REG] = 
{
    {AL, CL, DL, BL, AH, CH, DH, BH},
    {AX, CX, DX, BX, SP, BP, SI, DI},
    {CS, DS, SS, ES, -1, -1, -1, -1}
};

typedef u16 (*get_reg_func_ptr)(reg_mem_t *, u8);

typedef struct reg_mem
{
    u8 memory[BYTES_IN_KB * KB_IN_MB];
} reg_mem_t;

static void InitSegRegs(reg_mem_t * reg_mem);
static u16 MemoryGetMemoryValue(reg_mem_t *reg_mem, u8 segment, u16 offset);

reg_mem_t *MemoryCreate(void)
{
    reg_mem_t *reg_mem = (reg_mem_t *)calloc(1, sizeof(*reg_mem));
    if (!reg_mem)
    {
        perror("Failed to init reg_mem");
        exit(1);
    }
    
    InitSegRegs(reg_mem);

    return reg_mem;
}

void MemoryDestroy(reg_mem_t *reg_mem)
{
    assert(reg_mem != NULL);

    free(reg_mem);
}

static void InitSegRegs(reg_mem_t * reg_mem)
{
    reg_mem->memory[FLAGS_REG] = 0;
    reg_mem->memory[CS] = -1;
    reg_mem->memory[DS] = -1;
    reg_mem->memory[SS] = -1;
    reg_mem->memory[ES] = -1;
    reg_mem->memory[IP] = 0;
}

s64 MemorySetupCodeSeg(reg_mem_t *reg_mem, FILE *bin)
{
    assert(reg_mem);
    assert((s8)reg_mem->memory[CS] == -1);

    u16 first_available_addr = 28;
    reg_mem->memory[CS] = first_available_addr;

    u16 data = 0;
    for (u32 offset = first_available_addr;
         fread(&data, sizeof(data), 1, bin) == 1;
         offset += 2)
    {
        *(u16 *)&reg_mem->memory[offset] = data;
    }

    if (ferror(bin))
    {
        perror("MemorySetupCodeSeg");
        exit(1);
    }

    return ftell(bin);
}

void MemorySetWordRegValue(reg_mem_t *reg_mem, u8 reg, u16 value)
{
    assert(reg_mem);
    assert(reg < NUM_GENERAL_PURPOSE_REG);

    u8 reg_trans = register_translation_table[WORD][reg];
    u16 *where = (u16 *)&(reg_mem->memory[reg_trans]);
    *where = value;
}

void MemorySetByteRegValue(reg_mem_t *reg_mem, u8 reg, u16 value)
{
    assert(reg_mem);
    assert(reg < BYTE_REGS);

    u8 reg_trans = register_translation_table[BYTE][reg];
    reg_mem->memory[reg_trans] = value;
}

u16 MemoryGetWordRegValue(reg_mem_t *reg_mem, u8 reg)
{
    assert(reg_mem);
    assert(reg < WORD_REGS);

    u8 reg_trans = register_translation_table[WORD][reg];
    return *(u16 *)&(reg_mem->memory[reg_trans]);
}

u16 MemoryGetByteRegValue(reg_mem_t *reg_mem, u8 reg)
{
    assert(reg_mem);
    assert(reg < BYTE_REGS);

    u8 reg_trans = register_translation_table[BYTE][reg];
    return reg_mem->memory[reg_trans];
}

u16 MemoryGetNextNByteMemory(reg_mem_t *reg_mem, u8 segment, u8 n)
{
    assert(reg_mem);
    assert(segment >= 0 && segment < NUM_SEGMENTS);
    assert(n > 0 && n <= 2);

    u16 ip = *(u16 *)&reg_mem->memory[IP];
    u16 bytes = MemoryGetMemoryValue(reg_mem, CODE_SEG, ip); 
    MemoryChangeIPByN(reg_mem, n);

    return bytes;
}

static u16 MemoryGetMemoryValue(reg_mem_t *reg_mem, u8 segment, u16 offset)
{
    u8 segment_trans = register_translation_table[SEGMENT][segment];
    u16 segment_base = reg_mem->memory[segment_trans];
    return *(u16 *)&(reg_mem->memory[segment_base + offset]);
}

void MemoryChangeIPByN(reg_mem_t *reg_mem, u16 n)
{
    assert(reg_mem);
    *(u16 *)&reg_mem->memory[IP] += n;
}

void MemoryFlagOn(reg_mem_t *reg_mem, u8 flag)
{
    assert(reg_mem);
    reg_mem->memory[FLAGS_REG] |= flag_mask_table[flag];
}

void MemoryFlagOff(reg_mem_t *reg_mem, u8 flag)
{
    assert(reg_mem);
    reg_mem->memory[FLAGS_REG] &= ~flag_mask_table[flag];
}

u8 MemoryGetFlag(reg_mem_t *reg_mem, u8 flag)
{
    assert(reg_mem);
    return (reg_mem->memory[FLAGS_REG] & flag_mask_table[flag]) >> flag_index_table[flag];
}

void MemoryPrintSingleReg(reg_mem_t *reg_mem, u8 size, u8 reg, enum befor_after_exec when)
{
    char *reg_table[REG_TYPES][NUM_GENERAL_PURPOSE_REG] = 
    {
        // byte (0)
        {"al", "cl", "dl", "bl", 
         "ah", "ch", "dh", "bh"},
        // word (1)
        {"ax", "cx", "dx", "bx",
         "sp", "bp", "si", "di"}
    };

    get_reg_func_ptr reg_getters[NUM_REG_GETTERS] =
    {
        MemoryGetByteRegValue, MemoryGetWordRegValue
    };
    
    static char buff[50];

    if (when == BEFOR_EXEC)
    {
        sprintf(buff, " ; %s: (0x%x)->", reg_table[size][reg], reg_getters[size](reg_mem, reg));
        return;
    }
    sprintf(&buff[strlen(buff)], "(0x%x)", reg_getters[size](reg_mem, reg));
    printf("%s", buff);
}

void MemoryPrintIPReg(reg_mem_t *reg_mem,  enum befor_after_exec when)
{
    static char buff[50];

    if (when == BEFOR_EXEC)
    {
        sprintf(buff, " ; IP: (0x%x)->", *(u16*)&reg_mem->memory[IP]);
        return;
    }
    sprintf(&buff[strlen(buff)], "(0x%x)", *(u16*)&reg_mem->memory[IP]);
    printf("%s", buff);
}

void MemoryPrintFlags(reg_mem_t *reg_mem)
{
    printf(" ZF: %d, SF: %d", MemoryGetFlag(reg_mem, ZF), MemoryGetFlag(reg_mem, SF));
}

void MemoryPrintAllReg(reg_mem_t *reg_mem)
{
    printf("ax - 0x%x (%d)\n", *(u16 *)(reg_mem->memory + AX), *(u16 *)(reg_mem->memory + AX));
    printf("bx - 0x%x (%d)\n", *(u16 *)(reg_mem->memory + BX), *(u16 *)(reg_mem->memory + BX));
    printf("cx - 0x%x (%d)\n", *(u16 *)(reg_mem->memory + CX), *(u16 *)(reg_mem->memory + CX));
    printf("dx - 0x%x (%d)\n", *(u16 *)(reg_mem->memory + DX), *(u16 *)(reg_mem->memory + DX));
    printf("sp - 0x%x (%d)\n", *(u16 *)(reg_mem->memory + SP), *(u16 *)(reg_mem->memory + SP));
    printf("bp - 0x%x (%d)\n", *(u16 *)(reg_mem->memory + BP), *(u16 *)(reg_mem->memory + BP));
    printf("si - 0x%x (%d)\n", *(u16 *)(reg_mem->memory + SI), *(u16 *)(reg_mem->memory + SI));
    printf("di - 0x%x (%d)\n\n", *(u16 *)(reg_mem->memory + DI), *(u16 *)(reg_mem->memory + DI));

    printf("al - 0x%x (%d)\n", reg_mem->memory[AL], reg_mem->memory[AL]);
    printf("ah - 0x%x (%d)\n", reg_mem->memory[AH], reg_mem->memory[AH]);
    printf("bl - 0x%x (%d)\n", reg_mem->memory[BL], reg_mem->memory[BL]);
    printf("bh - 0x%x (%d)\n", reg_mem->memory[BH], reg_mem->memory[BH]);
    printf("cl - 0x%x (%d)\n", reg_mem->memory[CL], reg_mem->memory[CL]);
    printf("ch - 0x%x (%d)\n", reg_mem->memory[CH], reg_mem->memory[CH]);
    printf("dl - 0x%x (%d)\n", reg_mem->memory[DL], reg_mem->memory[DL]);
    printf("dh - 0x%x (%d)\n", reg_mem->memory[DH], reg_mem->memory[DH]);
}
