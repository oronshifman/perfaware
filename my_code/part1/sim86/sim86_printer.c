#include <stdio.h>
#include <string.h>

#include "sim86_printer.h"
#include "sim86_clock_estimator.h"
#include "sim86_decoder.h"

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

void PrinterPrintClocks(expression_t *instruction)
{
    // TODO(15.7.24): impl
    char buff[50];

    sprintf(buff, "%d", instruction->clock_estimation.base_clocks);

    if (instruction->clock_estimation.ea_clocks != NONE)
    {
        sprintf(&buff[strlen(buff)], " + %d(EA)", instruction->clock_estimation.ea_clocks);
    }

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

void PrinterPrintDest(reg_mem_t *reg_mem, expression_t *instruction, enum befor_after_exec when)
{
    typedef u16 (*get_reg_func_ptr)(reg_mem_t *, u8);

    get_reg_func_ptr reg_getters[NUM_REG_GETTERS] =
    {
        MemoryGetByteRegValue, MemoryGetWordRegValue
    };
    
    static char buff[50];

    operand_t dest = instruction->operands[DEST];
    if (when == BEFOR_EXEC)
    {
        // sprintf(buff, " ; %s: (0x%x)->", reg_table[size][reg], reg_getters[size](reg_mem, reg));
        switch (dest.operand_type)
        {
            case REGISTER:
            {
                sprintf(buff, "; %s: (0x%x)->", reg_table[dest.size][dest.reg_code], reg_getters[dest.size](reg_mem, dest.reg_code));
            } break;
            
            case DIRECT_ADDR:
            {
                sprintf(buff, "; %d: (0x%x)->", dest.disp, MemoryGetMemoryValue(reg_mem, DATA_SEG, dest.disp));
            } break;

            case EFFECTIVE_ADDR:
            {
                sprintf(buff, "; %s", ea_calc[dest.ea_code]);
                if (dest.disp)
                {
                    sprintf(&buff[strlen(buff)], " + %d: (0x%x)->", dest.disp, MemoryGetMemoryValue(reg_mem, DATA_SEG, MemoryGetEAValue(reg_mem, dest.ea_code) + dest.disp));
                    break;
                }
                sprintf(&buff[strlen(buff)], ": (0x%x)->", MemoryGetMemoryValue(reg_mem, DATA_SEG, MemoryGetEAValue(reg_mem, dest.ea_code) + dest.disp));
            } break;
            
            default:
            {
                sprintf(buff, ";");
            } break;
        }
        return;
    }
    // sprintf(&buff[strlen(buff)], "(0x%x);", reg_getters[size](reg_mem, reg));
    switch (dest.operand_type)
    {
        case REGISTER:
        {
            sprintf(&buff[strlen(buff)], "(0x%x);", reg_getters[dest.size](reg_mem, dest.reg_code));
        } break;
        
        case DIRECT_ADDR:
        {
            sprintf(&buff[strlen(buff)], "(0x%x)", MemoryGetMemoryValue(reg_mem, DATA_SEG, dest.disp));
        } break;

        case EFFECTIVE_ADDR:
        {
            if (dest.disp)
            {
                sprintf(&buff[strlen(buff)], "(0x%x);", MemoryGetMemoryValue(reg_mem, DATA_SEG, MemoryGetEAValue(reg_mem, dest.ea_code) + dest.disp));
                break;
            }
            sprintf(&buff[strlen(buff)], "(0x%x);", MemoryGetMemoryValue(reg_mem, DATA_SEG, MemoryGetEAValue(reg_mem, dest.ea_code) + dest.disp));
        } break;
        
        default:
        {
        } break;
    }
    printf("%s", buff);
}

void PrinterPrintIPReg(reg_mem_t *reg_mem,  enum befor_after_exec when)
{
    static char buff[50];

    if (when == BEFOR_EXEC)
    {
        sprintf(buff, " ; IP: (0x%x)->", MemoryGetIP(reg_mem));
        return;
    }
    sprintf(&buff[strlen(buff)], "(0x%x)", MemoryGetIP(reg_mem));
    printf("%s", buff);
}

void PrinterPrintFlags(reg_mem_t *reg_mem)
{
    printf(" ZF: %d, SF: %d", MemoryGetFlag(reg_mem, ZF), MemoryGetFlag(reg_mem, SF));
}

void PrinterPrintAllReg(reg_mem_t *reg_mem)
{
    printf("ax - 0x%x (%d)\n", MemoryGetWordRegValue(reg_mem, 0), MemoryGetWordRegValue(reg_mem, 0));
    printf("bx - 0x%x (%d)\n", MemoryGetWordRegValue(reg_mem, 3), MemoryGetWordRegValue(reg_mem, 3));
    printf("cx - 0x%x (%d)\n", MemoryGetWordRegValue(reg_mem, 1), MemoryGetWordRegValue(reg_mem, 1));
    printf("dx - 0x%x (%d)\n", MemoryGetWordRegValue(reg_mem, 2), MemoryGetWordRegValue(reg_mem, 2));
    printf("sp - 0x%x (%d)\n", MemoryGetWordRegValue(reg_mem, 4), MemoryGetWordRegValue(reg_mem, 4));
    printf("bp - 0x%x (%d)\n", MemoryGetWordRegValue(reg_mem, 5), MemoryGetWordRegValue(reg_mem, 5));
    printf("si - 0x%x (%d)\n", MemoryGetWordRegValue(reg_mem, 6), MemoryGetWordRegValue(reg_mem, 6));
    printf("di - 0x%x (%d)\n\n", MemoryGetWordRegValue(reg_mem, 7), MemoryGetWordRegValue(reg_mem, 7));

    printf("al - 0x%x (%d)\n", MemoryGetByteRegValue(reg_mem, 0), MemoryGetByteRegValue(reg_mem, 0));
    printf("ah - 0x%x (%d)\n", MemoryGetByteRegValue(reg_mem, 3), MemoryGetByteRegValue(reg_mem, 3));
    printf("bl - 0x%x (%d)\n", MemoryGetByteRegValue(reg_mem, 1), MemoryGetByteRegValue(reg_mem, 1));
    printf("bh - 0x%x (%d)\n", MemoryGetByteRegValue(reg_mem, 2), MemoryGetByteRegValue(reg_mem, 2));
    printf("cl - 0x%x (%d)\n", MemoryGetByteRegValue(reg_mem, 4), MemoryGetByteRegValue(reg_mem, 4));
    printf("ch - 0x%x (%d)\n", MemoryGetByteRegValue(reg_mem, 7), MemoryGetByteRegValue(reg_mem, 7));
    printf("dl - 0x%x (%d)\n", MemoryGetByteRegValue(reg_mem, 6), MemoryGetByteRegValue(reg_mem, 6));
    printf("dh - 0x%x (%d)\n", MemoryGetByteRegValue(reg_mem, 5), MemoryGetByteRegValue(reg_mem, 5));
}