#ifndef __SIM86_MANAGER_H__
#define __SIM86_MANAGER_H__

#include <stdint.h>
#include <stdio.h>

enum option_types
{
    EXEC_BIN,
    PRINT_TO_ASM,

    NUM_OPTIONS
};

void ManagerDecodeBin(FILE *bin, uint8_t option);
uint8_t ManagerParseOption(char *option);

#endif /* __SIM86_MANAGER_H__ */