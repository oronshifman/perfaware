#ifndef __SIM86_INSTRUCTION_H__
#define __SIM86_INSTRUCTION_H__

#include "my_int.h"

// NOTE: MOV
// NOTE: Register/memory to/from register
#define MOV_RM_TO_FROM_REG 0b10001000

// NOTE: Immediate to register/memory
#define MOV_IMMEDIATE_TO_RM 0b11000100

// NOTE: Immediate to register
#define MOV_IMMEDIATE_TO_REG 0b10110000
#define MOV_IMMEDIATE_TO_REG_W 0b10111000
#define MOV_IMMEDIATE_TO_REG_REG 0b10110100
#define MOV_IMMEDIATE_TO_REG_W_REG 0b10111100
    
#define ADD_SUB_CMP_IMMEDIATE_TO_RM 0b10000000 // determine the kind of op to be done

#define ADD_RM_TO_FROM_REG 0b00000000
#define ADD_IMMEDIATE_TO_RM 0b000 // reg field
#define ADD_IMMEDIATE_TO_ACC 0b00000100

#define SUB_RM_TO_FROM_REG 0b00101000
#define SUB_IMMEDIATE_TO_RM 0b101 // reg field
#define SUB_IMMEDIATE_TO_ACC 0b00101100

#define CMP_RM_TO_FROM_REG 0b00111000
#define CMP_IMMEDIATE_TO_RM 0b111 // reg field
#define CMP_IMMEDIATE_TO_ACC 0b00111100

#define JO 0b01110000
#define JNO 0b01110001
#define JB 0b01110010
#define JNB 0b01110011
#define JE 0b01110100
#define JNE 0b01110101
#define JBE 0b01110110
#define JA 0b01110111
#define JS 0b01111000
#define JNS 0b01111001
#define JP 0b01111010
#define JNP 0b01111011
#define JL 0b01111100
#define JNL 0b01111101
#define JLE 0b01111110
#define JG 0b01111111
#define LOOPNZ 0b11100000
#define LOOPZ 0b11100001
#define LOOP 0b11100010
#define JCXZ 0b11100011

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