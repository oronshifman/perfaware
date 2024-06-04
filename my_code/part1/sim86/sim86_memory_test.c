#include <stdio.h>

#include "sim86_memory.h"

int main(void)
{
    reg_mem_t *memory = InitMemory();
    SetByteRegValue(memory, AL, 0xcc);
    SetByteRegValue(memory, AH, 0xff);
    SetWordRegValue(memory, BX, 0xffcc);

    PrintRegistersState(memory);
    
    return 0;
}