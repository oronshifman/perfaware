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

static u8 flag_mask_table[NUM_FLAGS] =
{
    ZF_MASK, SF_MASK
};

static u8 flag_index_table[NUM_FLAGS] =
{
    ZF_INDEX, SF_INDEX
};

static u8 register_translation_table[REG_TYPES][NUM_GENERAL_PURPOSE_REG] = 
{
    {AL, CL, DL, BL, AH, CH, DH, BH},
    {AX, CX, DX, BX, SP, BP, SI, DI},
    {CS, DS, SS, ES, -1, -1, -1, -1}
};

typedef struct reg_mem
{
    u8 memory[BYTES_IN_KB * KB_IN_MB];
} reg_mem_t;

static void InitSegRegs(reg_mem_t * reg_mem);

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
    *(u16 *)&reg_mem->memory[DS] = 5000;
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

    u8 data = 0;
    for (u32 offset = first_available_addr;
         fread(&data, sizeof(data), 1, bin) == 1;
         ++offset)
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

void MemoryDump(reg_mem_t *reg_mem, u32 from, u16 size)
{
    FILE *mem_dump = fopen("mem_dump.data", "w");
    u16 segment_base = *(u16 *)&reg_mem->memory[DS];
    fwrite(&reg_mem->memory[segment_base], size, 1, mem_dump);
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

u16 MemoryGetMemoryValue(reg_mem_t *reg_mem, u8 segment, u16 offset)
{
    u8 segment_trans = register_translation_table[SEGMENT][segment];
    u16 segment_base = *(u16 *)&reg_mem->memory[segment_trans];
    return *(u16 *)&reg_mem->memory[segment_base + offset];
}

u16 MemoryGetEAValue(reg_mem_t *reg_mem, u8 ea_code)
{
    u32 ea_table[] =
    {
        *(u16 *)&(reg_mem->memory[BX]) + *(u16 *)&(reg_mem->memory[SI]),
        *(u16 *)&(reg_mem->memory[BX]) + *(u16 *)&(reg_mem->memory[DI]),
        *(u16 *)&(reg_mem->memory[BP]) + *(u16 *)&(reg_mem->memory[SI]),
        *(u16 *)&(reg_mem->memory[BP]) + *(u16 *)&(reg_mem->memory[DI]),
        *(u16 *)&(reg_mem->memory[SI]),
        *(u16 *)&(reg_mem->memory[DI]),
        *(u16 *)&(reg_mem->memory[BP]),
        *(u16 *)&(reg_mem->memory[BX]),
    };

    return ea_table[ea_code];
}

void MemorySetMemoryValue(reg_mem_t *reg_mem, u8 segment, u16 offset, u16 data)
{
    u8 segment_trans = register_translation_table[SEGMENT][segment];
    u16 segment_base = *(u16 *)&reg_mem->memory[segment_trans];
    *(u16 *)&reg_mem->memory[segment_base + offset] = data;
}

u16 MemoryGetIP(reg_mem_t *reg_mem)
{
    return *(u16 *)&reg_mem->memory[IP];
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
