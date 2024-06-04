#ifndef __SIM86_DECODER_H__
#define __SIM86_DECODER_H__

#include <stdio.h>

#include "sim86_instruction.h"

enum operand_type
{
    REGISTER,
    EFFECTIVE_ADDR,
    MEMORY,
    JUMP_OFFSET,
    IMMEDIATE,

    NUM_OPERAND_TYPES
};

typedef struct operand
{
    enum operand_type operand_type;
    uint8_t size; // NOTE: 1 for word 0 for byte
    union
    {
        uint8_t reg_code;
        uint8_t ea_code;
        int32_t signed_immediate; // NOTE: if JMP this will be initialized
        uint32_t unsigned_immediate;
    };    
    uint16_t disp;
} operand_t;

typedef struct expresion
{
    enum operation_type operation_type;
    operand_t dest;
    operand_t src;
} expresion_t;

/*
    @ prams: 
        instruction: pointer to an instruction struct to be feeled by decoder.
        bin:         binary file to read from.
*/
int GetNextInstruction(expresion_t *decoded_inst, FILE *bin);

#endif /* __SIM86_DECODER_H__ */

// compile only with decoder  for testing it
// gcc -I. sim86_main.c sim86_decoder.c sim86_manager.c sim86_instruction.c -o test -g
