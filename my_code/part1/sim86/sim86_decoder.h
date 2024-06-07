#ifndef __SIM86_DECODER_H__
#define __SIM86_DECODER_H__

#include <stdio.h>

#include "sim86_instruction.h"

enum operand_type
{
    REGISTER,
    EFFECTIVE_ADDR,
    DIRECT_ADDR,
    JUMP_CODE,
    JUMP_OFFSET,
    IMMEDIATE,

    NUM_OPERAND_TYPES
};

enum direction
{
    DEST,
    SRC,

    NUM_OPERANDS
};

typedef struct operand
{
    enum direction direction;
    enum operand_type operand_type;
    uint8_t size; // NOTE: 1 for word 0 for byte
    union
    {
        uint8_t jmp_code;
        uint8_t reg_code;
        uint8_t ea_code;
        int8_t jmp_offset; // NOTE: if JMP, this will be initialized as jump offset
        uint16_t unsigned_immediate;
    };    
    uint16_t disp; // NOTE: this is out of the union because it's needed WITH ea_code
} operand_t;

typedef struct expression
{
    enum operation_type operation_type;
    operand_t operands[NUM_OPERANDS]; // NOTE: if JMP, <jmp_code in dest> and <jmp offset is in src.signed_immediate>
} expression_t;

/*
    @ prams: 
        instruction: pointer to an instruction struct to be feeled by decoder.
        bin:         binary file to read from.
*/
int GetNextInstruction(expression_t *decoded_inst, FILE *bin);

#endif /* __SIM86_DECODER_H__ */

// compile only with decoder  for testing it
// gcc -I. sim86_main.c sim86_decoder.c sim86_manager.c sim86_instruction.c -o test -g
