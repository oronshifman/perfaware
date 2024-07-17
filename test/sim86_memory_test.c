#include <stdio.h>

#include "my_int.h"
#include "sim86_memory.h"

void TestFlags();
void TestCodeSegment(u8 *file_name);

u32 main(u32 argc, u8 *argv[])
{
    TestFlags();
    TestCodeSegment(argv[1]);
    return 0;
}

void TestCodeSegment(u8 *file_name)
{
    FILE *bin = fopen(file_name, "rb");
    if (!bin)
    {
        perror("Open file");
        return;
    }

    reg_mem_t *reg_mem = MemoryCreate();
    MemorySetupCodeSeg(reg_mem, bin);

    u32 bin_size = ftell(bin);

    for (u32 i = 0; i < bin_size;)
    {
        printf("%02d  ", i);
        for (u8 line = 0; line < 10; ++line, i += 2)
        {
            u16 from_mem = MemoryGetMemoryValue(reg_mem, CODE_SEG, i);
            printf("%04x ", from_mem);
        }
        printf("\n");
    }
}

void TestFlags()
{
    reg_mem_t *reg_mem = MemoryCreate();
    MemoryFlagOn(reg_mem, ZF);
    MemoryFlagOn(reg_mem, SF);

    printf("ZF: %d\n", MemoryGetFlag(reg_mem, ZF));
    printf("SF: %d\n", MemoryGetFlag(reg_mem, SF));

    MemoryFlagOff(reg_mem, ZF);
    MemoryFlagOff(reg_mem, SF);

    printf("ZF: %d\n", MemoryGetFlag(reg_mem, ZF));
    printf("SF: %d\n", MemoryGetFlag(reg_mem, SF));
}