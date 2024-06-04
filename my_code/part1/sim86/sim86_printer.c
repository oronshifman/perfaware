#include <stdio.h>
#include <string.h>

#include "sim86_printer.h"

#define NUM_EA 8
#define NUM_REG 8
#define NUM_REG_SIZES 2

enum mods
{
    EA_0DISP,
    EA_8DISP,
    EA_16DISP,
    TO_REG
};

static uint8_t *op_nems[NUM_OPERATION_TYPES] = 
{
    "", "mov", "add", "sub", "cmp", "jmp"
};

static uint8_t *reg_table[NUM_REG_SIZES][NUM_REG] = 
{
    // byte (0)
    {"al", "cl", "dl", "bl", 
     "ah", "ch", "dh", "bh"},
    // word (1)
    {"ax", "cx", "dx", "bx",
     "sp", "bp", "si", "di"}
};

static uint8_t *ea_calc[NUM_EA] = 
{
    "bx + si", "bx + di", "bp + si", "bp + di",
    "si", "di", "bp","bx"
};  

void PrintInstructionAsm(decoded_inst_t *instruction)
{
    char buff[50];

    if (instruction->op_type == JMP)
    {
        // TODO: handle printing the jumps!!!!
        sprintf(buff, "%s ", op_nems[instruction->op_type]);
        sprintf(&buff[strlen(buff)], "%d", instruction->dest.value);
        printf("%s\n", buff);
        return;
    }
    
    sprintf(buff, "%s ", op_nems[instruction->op_type]);

    // NOTE: print dest
    if (instruction->dest.field_type == RM)
    {
        sprintf(&buff[strlen(buff)], "[%s", ea_calc[instruction->dest.value]);
        
        if (instruction->disp.state == INITIALIZED)
        {
            sprintf(&buff[strlen(buff)], " + %d], ", instruction->disp.value);
        }
        else
        {
            sprintf(&buff[strlen(buff)], "], ");
        }
    }
    else if (instruction->dest.field_type == DISP)
    {
        sprintf(&buff[strlen(buff)], "[%d], ", instruction->dest.value);
    }
    else
    {
        sprintf(&buff[strlen(buff)], "%s, ", reg_table[instruction->w.value][instruction->dest.value]);
    }   

    // NOTE: print src
    if (instruction->src.field_type == RM)
    {
        sprintf(&buff[strlen(buff)], "[%s", ea_calc[instruction->src.value]);
        if (instruction->disp.state == INITIALIZED)
        {
            sprintf(&buff[strlen(buff)], " + %d]", instruction->disp.value);
        }
        else
        {
            sprintf(&buff[strlen(buff)], "]");
        }
    }
    else
    {
        if (instruction->src.field_type == DATA)
        {
            sprintf(&buff[strlen(buff)], "%d", instruction->src.value);    
        }
        else
        {
            sprintf(&buff[strlen(buff)], "%s", reg_table[instruction->w.value][instruction->src.value]);
        }
    }
  
    printf("%s\n", buff);
}