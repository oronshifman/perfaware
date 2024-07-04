#ifndef __SIM86_MANAGER_H__
#define __SIM86_MANAGER_H__

#include <stdio.h>
#include "my_int.h"

enum option_types
{
    EXEC_BIN,
    DUMP_MEM,
    PRINT_TO_ASM,

    NUM_OPTIONS
};

void ManagerOperate(FILE *bin, u8 option);
u8 ManagerParseOption(char *option);

#endif /* SIM86_MANAGER_H */