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

typedef void (*option_func_ptr)(expression_t *, reg_mem_t *, s64 *);

static void Print(expression_t *expression, reg_mem_t *reg_mem, s64 *code_left);
static void Exec(expression_t *expression, reg_mem_t *reg_mem, s64 *code_left);
static void GetNextInstruction(expression_t *expression, reg_mem_t *reg_mem, s64 *code_left);

void ManagerOperate(FILE *bin, u8 option)
{
    assert(bin);

    reg_mem_t *reg_mem = MemoryCreate();
    s64 code_left = MemorySetupCodeSeg(reg_mem, bin);

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
    while (code_left) 
    {
        option_func(decoded_inst, reg_mem, &code_left);
    }
    
    if (option == EXEC_BIN)
    {
        printf("\n\n");
        MemoryPrintAllReg(reg_mem);
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

static void Print(expression_t *expression, reg_mem_t *reg_mem, s64 *code_left)
{
    GetNextInstruction(expression, reg_mem, code_left);

    PrinterPrintInst(expression);
    printf("\n");
}

static void Exec(expression_t *expression, reg_mem_t *reg_mem, s64 *code_left)
{
    // print IP value before decode
    MemoryPrintIPReg(reg_mem, BEFOR_EXEC);
    
    // decode instruction
    GetNextInstruction(expression, reg_mem, code_left);
    
    // print instruction disassembly
    PrinterPrintInst(expression);
    
    // print modified register value before exec
    MemoryPrintSingleReg(reg_mem, expression->operands[DEST].size, expression->operands[DEST].reg_code, BEFOR_EXEC);
    
    // print IP value after decode
    MemoryPrintIPReg(reg_mem, AFTER_EXEC);
    
    // execute instruction
    ExecutorExecInst(expression, reg_mem);
    
    // print modified register value after exec
    MemoryPrintSingleReg(reg_mem, expression->operands[DEST].size, expression->operands[DEST].reg_code, AFTER_EXEC);
    
    // print flags values after exec
    MemoryPrintFlags(reg_mem);
    printf("\n");
}

/**
 * decodes the next instruction and updated code_left to (code_left - next instruction length)
 */
static void GetNextInstruction(expression_t *expression, reg_mem_t *reg_mem, s64 *code_left)
{
    u8 inst_len = DecoderGetNextInst(expression, reg_mem);
    *code_left -= inst_len;
}