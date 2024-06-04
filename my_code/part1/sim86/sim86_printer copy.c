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

static void PrintEAZeroDisp(inst_t *instruction);
static void PrintEAWithDisp(inst_t *instruction);
static void PrintToReg(inst_t *instruction);
static void PrintImmediateToReg(inst_t *instruction);

void PrintInstructionAsm(inst_t *instruction)
{
    if (instruction->field[MOD].state == NOT_USED)
    {
        PrintImmediateToReg(instruction);
        return;
    }

    uint8_t mod = instruction->field[MOD].value;
    
    switch (mod)
    {
        case EA_0DISP:
        {
            PrintEAZeroDisp(instruction);
        } break;

        case EA_8DISP:
        case EA_16DISP:
        {
            PrintEAWithDisp(instruction); 
        } break;

        case TO_REG:
        {
            PrintToReg(instruction);  
        } break;
    
    default:
        break;
    }
}

static void PrintEAZeroDisp(inst_t * instruction)
{
    uint8_t inst_op = instruction->op_type;
    uint8_t reg = instruction->field[REG].value;
    uint8_t rm = instruction->field[RM].value;

    uint8_t d = instruction->field[D].value;
    uint8_t w = instruction->field[W].value;

    uint16_t data = instruction->field[DATA].value;

    char buf[50];

    sprintf(buf, "%s ", op_nems[inst_op]);

    if (instruction->field[D].state == NOT_USED)
    {
        sprintf(&buf[strlen(buf)], "[%s], %d", ea_calc[rm], data);
    }
    else if (d)
    {
        // TODO: do with sprintf to handle last two instructions in listring_39 (line 37-38)
        sprintf(&buf[strlen(buf)], "%s, [%s]", reg_table[w][reg], ea_calc[rm]);
    }
    else 
    {
        // TODO: do with sprintf to handle last two instructions in listring_39 (line 37-38)
        sprintf(&buf[strlen(buf)], "[%s], %s", ea_calc[rm], reg_table[w][reg]);
    }

    printf("%s\n", buf);
}

static void PrintEAWithDisp(inst_t * instruction)
{
    uint8_t inst_op = instruction->op_type;
    uint8_t reg = instruction->field[REG].value;
    uint8_t rm = instruction->field[RM].value;

    uint8_t d = instruction->field[D].value;
    uint8_t w = instruction->field[W].value;
    uint8_t mod = instruction->field[MOD].value;

    uint16_t disp = instruction->field[DISP].value;
    uint16_t data = instruction->field[DATA].value;  

    char buf[50];

    if (instruction->field[D].state == NOT_USED)
    {
        sprintf(buf, "%s ", op_nems[inst_op]);

        sprintf(&buf[strlen(buf)], "[%s ", ea_calc[rm]);

        disp == 0 ? sprintf(&buf[strlen(buf)], "] ") : 
                    sprintf(&buf[strlen(buf)], "+ %d] ", disp);
        
        sprintf(&buf[strlen(buf)], "%d", data);
    }
    else if (d)
    {
        sprintf(buf, "%s %s, [%s", op_nems[inst_op], reg_table[w][reg], ea_calc[rm]);
        disp == 0 ? sprintf(&buf[strlen(buf)], "]") : 
                    sprintf(&buf[strlen(buf)], " + %d]", disp);
    }
    else 
    {
        sprintf(buf, "%s [%s", op_nems[inst_op], ea_calc[rm]);
        disp == 0 ? sprintf(&buf[strlen(buf)], "], %s", reg_table[w][reg]) : 
                    sprintf(&buf[strlen(buf)], " + %d], %s", disp, reg_table[w][reg]);
    }
    
    printf("%s\n", buf);
}

static void PrintToReg(inst_t * instruction)
{
    uint8_t inst_op = instruction->op_type;
    uint8_t reg = instruction->field[REG].value;
    uint8_t rm = instruction->field[RM].value;

    uint8_t w = instruction->field[W].value;

    char buf[50];
    
    sprintf(buf, "%s ", op_nems[inst_op]);

    if (instruction->field[REG].state == NOT_USED)
    {
        sprintf(&buf[strlen(buf)], "%s %d", reg_table[w][rm], 
                                            instruction->field[DATA].value);

    }
    else
    {
        sprintf(&buf[strlen(buf)], "%s, %s", reg_table[w][rm], reg_table[w][reg]);
    }

    printf("%s\n", buf);
}

static void PrintImmediateToReg(inst_t * instruction)
{
    uint8_t inst_op = instruction->op_type;
    uint8_t reg = instruction->field[REG].value;

    uint8_t w = instruction->field[W].value;
    uint16_t data = instruction->field[DATA].value;

    printf("%s %s, %d\n", op_nems[inst_op], 
                          reg_table[w][reg],
                          data);
}