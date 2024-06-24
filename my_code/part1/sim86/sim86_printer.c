#include <stdio.h>
#include <string.h>

#include "sim86_printer.h"

#define NUM_EA 8
#define NUM_REG 8
#define NUM_REG_SIZES 2
#define NUM_JMP_TYPES 20

enum mods
{
    EA_0DISP,
    EA_8DISP,
    EA_16DISP,
    TO_REG
};

static char *op_nems[NUM_OPERATION_TYPES] = 
{
    "", "mov", "add", "sub", "cmp", ""
};

static char *jmp_types[NUM_JMP_TYPES] = 
{
    "jo", "jno", "jb", "jnb", "je",
    "jne", "jbe", "ja", "js", "jns",
    "jp", "jnp", "jl", "jnl", "jle",
    "jg", "loopnz", "loopz", "loop", "jcxz"
};

static char *reg_table[NUM_REG_SIZES][NUM_REG] = 
{
    // byte (0)
    {"al", "cl", "dl", "bl", 
     "ah", "ch", "dh", "bh"},
    // word (1)
    {"ax", "cx", "dx", "bx",
     "sp", "bp", "si", "di"}
};

static char *ea_calc[NUM_EA] = 
{
    "bx + si", "bx + di", "bp + si", "bp + di",
    "si", "di", "bp","bx"
};  

static void AddOperandToBuff(char *buff, operand_t *operand);

void PrinterPrintInst(expression_t *instruction)
{
    char buff[50];

    if (instruction->operation_type == JMP)
    {
        sprintf(buff, "%s ", jmp_types[instruction->operands[DEST].jmp_code]);
        sprintf(&buff[strlen(buff)], "%d", instruction->operands[SRC].jmp_offset);
        printf("%s", buff);
        return;
    }
    
    sprintf(buff, "%s ", op_nems[instruction->operation_type]);

    AddOperandToBuff(buff, &instruction->operands[DEST]);
    sprintf(&buff[strlen(buff)], ", ");
    AddOperandToBuff(buff, &instruction->operands[SRC]);
 
    printf("%s", buff);
}

static void AddOperandToBuff(char *buff, operand_t *operand)
{
    switch (operand->operand_type)
    {
        case REGISTER:
        {
            sprintf(&buff[strlen(buff)], "%s", reg_table[operand->size][operand->reg_code]);
        } break;

        case EFFECTIVE_ADDR:
        {
            sprintf(&buff[strlen(buff)], "[%s", ea_calc[operand->ea_code]);
            if (operand->disp)
            {
                sprintf(&buff[strlen(buff)], " + %d]", operand->disp);
                break;
            }
            sprintf(&buff[strlen(buff)], "]");
        } break;

        case DIRECT_ADDR:
        {
            sprintf(&buff[strlen(buff)], "[%d]", operand->disp);
        } break;

        case IMMEDIATE:
        {
            sprintf(&buff[strlen(buff)], "%d", operand->unsigned_immediate);
        } break;

        default:
        {
            sprintf(&buff[strlen(buff)], " <--ERROR--> ");
        } break;
    }
}