#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "sim86_manager.h"
#include "sim86_decoder.h"
#include "sim86_printer.h"
#include "sim86_executer.h"
#include "sim86_memory.h"

char *str_options[NUM_OPTIONS] = 
{
    "-exec", "-print"
};

typedef void (*option_func_ptr)(expression_t *, reg_mem_t *);

static void Print(expression_t *expression, reg_mem_t *reg_mem);
static void Exec(expression_t *expression, reg_mem_t *reg_mem);

void ManagerDecodeBin(FILE *bin, uint8_t option)
{
    assert(bin);

    reg_mem_t *reg_mem;
    option_func_ptr option_func = NULL;
    switch (option)
    {
        case EXEC_BIN:
        {
            option_func = Exec;
            reg_mem = MemoryCreate();
        } break;

        case PRINT_TO_ASM:
        {
            option_func = Print;
        } break;
    }

    expression_t *decoded_inst = malloc(sizeof(*decoded_inst));
    while (DecoderGetNextInst(decoded_inst, bin)) 
    {
        option_func(decoded_inst, reg_mem);
    }
    
    if (option == EXEC_BIN)
    {
        printf("\n\n");
        MemoryPrintAllReg(reg_mem);
        MemoryDestroy(reg_mem);
    }
}

uint8_t ManagerParseOption(char *option)
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

static void Print(expression_t *expression, reg_mem_t *reg_mem)
{
    PrinterPrintInst(expression);
    printf("\n");
}

static void Exec(expression_t *expression, reg_mem_t *reg_mem)
{
    PrinterPrintInst(expression);
    MemoryPrintSingleReg(reg_mem, expression->operands[DEST].size, expression->operands[DEST].reg_code, BEFOR_EXEC);
    ExecutorExecInst(expression, reg_mem);
    MemoryPrintSingleReg(reg_mem, expression->operands[DEST].size, expression->operands[DEST].reg_code, AFTER_EXEC);
    printf("\n");
}