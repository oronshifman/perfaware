#include <stdio.h>

#include "sim86_memory.h"

void TestFlags();

int main(void)
{
    TestFlags();
    return 0;
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