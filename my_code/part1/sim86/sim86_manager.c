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

void ManagerOperate(FILE *bin, u8 option)
{
    assert(bin);

    reg_mem_t *reg_mem = MemoryCreate();
    s64 end_of_code = MemorySetupCodeSeg(reg_mem, bin);

    option_func_ptr option_func = NULL;
    switch (option)
    {
        case EXEC_BIN:
        {
            option_func = Exec;
        } break;

        case PRINT_TO_ASM:
        {
            option_func = Print;
        } break;
    }

    expression_t *decoded_inst = malloc(sizeof(*decoded_inst));
    while (end_of_code > MemoryGetIP(reg_mem)) 
    {
        option_func(decoded_inst, reg_mem);
    }
    
    if (option == EXEC_BIN)
    {
        printf("\n\n");
        PrinterPrintAllReg(reg_mem);
        MemoryDestroy(reg_mem);
    }
}

u8 ManagerParseOption(char *option)
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
    DecoderGetNextInst(expression, reg_mem);

    PrinterPrintInst(expression);
    printf("\n");
}

static void Exec(expression_t *expression, reg_mem_t *reg_mem)
{
    // print IP value before decode
    PrinterPrintIPReg(reg_mem, BEFOR_EXEC);
    
    // decode instruction
    DecoderGetNextInst(expression, reg_mem);
    
    // print instruction disassembly
    PrinterPrintInst(expression);
    
    // print modified register value before exec
    PrinterPrintDest(reg_mem, expression, BEFOR_EXEC);
    
    // print IP value after decode
    PrinterPrintIPReg(reg_mem, AFTER_EXEC);
    
    // execute instruction
    ExecutorExecInst(expression, reg_mem);
    
    // print modified register value after exec
    PrinterPrintDest(reg_mem, expression, AFTER_EXEC);
    
    // print flags values after exec
    PrinterPrintFlags(reg_mem);
    printf("\n");
}