#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "sim86_manager.h"
#include "sim86_decoder.h"
#include "sim86_printer.h"
#include "sim86_executer.h"

char *str_options[NUM_OPTIONS] = 
{
    "-exec", "-print"
};

typedef void (*option_func_ptr)(expresion_t *);
// printf("%d\n", __LINE__); // NOTE: print for debugging++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void DecodeBin(FILE *bin, uint8_t option)
{
    assert(bin);

    option_func_ptr option_func = NULL;
    switch (option)
    {
        case EXEC_BIN:
        {
            // TODO: impl ExecuteInstruction in executor modul
            option_func = ExecuteInstruction;
        } break;

        case PRINT_TO_ASM:
        {
            option_func = PrintInstructionAsm;
        } break;
    }

    expresion_t *decoded_inst = malloc(sizeof(*decoded_inst));
    while (GetNextInstruction(decoded_inst, bin)) 
    {
        option_func(decoded_inst);
    }
    
    if (option == EXEC_BIN)
    {
        PrintMemory();
    }
}

uint8_t ParseOption(char *option)
{
    assert(option);

    for (int option_type = 0; option_type < NUM_OPTIONS; ++option_type)
    {
        if (!strcmp(option, str_options[option_type]))
        {
            return option_type;
        }
    }

    return -1;
}