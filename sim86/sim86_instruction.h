#ifndef __SIM86_INSTRUCTION_H__
#define __SIM86_INSTRUCTION_H__

#include "my_int.h"



enum operation_type
{
    NONE, // for initializing to 0

    MOV,
    ADD,
    SUB,
    CMP,
    JMP,

    NUM_OPERATION_TYPES,
};

enum field_type
{
    D,
    W,
    S,
    MOD,
    REG,
    RM,
    JMP_OFFSET,
    DISP,
    DATA,

    NUM_FIELDS,
};

enum field_state
{
    NOT_USED,
    INITIALIZED,
    UNINITIALIZED
};

typedef struct inst_field
{
    enum field_type field_type;
    enum field_state state;
    u8 offset;         // field location in instruction, offset from LSB
    u8 mask;           // represents the number of bits in the field
    u16 value;
} inst_field_t;

typedef struct inst
{
    enum operation_type operation_type;        // NOTE: corresponds to enum operation_types from sim86_general.h
    inst_field_t field[NUM_FIELDS];
} inst_t;

void InstructionInitOpTable(inst_t *op_table);

#endif /* __SIM86_GENERAL_H__ */ 