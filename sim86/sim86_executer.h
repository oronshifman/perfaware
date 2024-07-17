#ifndef __SIM86_EXECUTER_H__
#define __SIM86_EXECUTER_H__

#include "sim86_decoder.h"
#include "sim86_memory.h"

void ExecutorExecInst(expression_t *instruction, reg_mem_t *reg_mem);

#endif /* __SIM86_EXECUTER_H__ */