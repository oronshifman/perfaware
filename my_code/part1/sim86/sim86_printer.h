#ifndef __SIM86_PRINTER_H__
#define __SIM86_PRINTER_H__

#include "sim86_decoder.h"

enum befor_after_exec
{
    BEFOR_EXEC,
    AFTER_EXEC
};

void PrinterPrintInst(expression_t *instruction);
void PrinterPrintClocks(expression_t *instruction);

void PrinterPrintAllReg(reg_mem_t *reg_mem);
void PrinterPrintDest(reg_mem_t *reg_mem, expression_t *instruction, enum befor_after_exec when);
void PrinterPrintFlags(reg_mem_t *reg_mem);
void PrinterPrintIPReg(reg_mem_t *reg_mem,  enum befor_after_exec when);

#endif /* SIM86_PRINTER_H */