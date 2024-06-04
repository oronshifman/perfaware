#ifndef __SIM86_INSTRUCTION_H__
#define __SIM86_INSTRUCTION_H__

#include <stdint.h>

enum operation_type
{
    NONE, // for initailizing to 0

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
    uint8_t offset;         // field location in instruction, offset from LSB
    uint8_t mask;           // represents the number of bits in the field
    uint16_t value;
} inst_field_t;

typedef struct inst
{
    enum operation_type operation_type;        // NOTE: coresponds to enum operation_types from sim86_general.h
    inst_field_t field[NUM_FIELDS];
} inst_t;

void InitOpTable(inst_t *op_table);

#endif /* __SIM86_GENERAL_H__ */ 