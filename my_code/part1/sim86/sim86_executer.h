#ifndef __SIM86_EXECUTER_H__
#define __SIM86_EXECUTER_H__

#include "sim86_decoder.h"

void ExecuteInstruction(expression_t *instruction);
void PrintMemory(void);

#endif /* __SIM86_EXECUTER_H__ */