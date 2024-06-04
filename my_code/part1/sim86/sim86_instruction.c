#include "sim86_instruction.h"

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

#define JE 0b01110100
#define JL 0b01111100
#define JLE 0b01111110
#define JB 0b01110010
#define JBE 0b01110110
#define JP 0b01111010
#define JO 0b01110000
#define JS 0b01111000
#define JNE 0b01110101
#define JNL 0b01111101
#define JG 0b01111111
#define JNB 0b01110011
#define JA 0b01110111
#define JNP 0b01111011
#define JNO 0b01110001
#define JNS 0b01111001
#define LOOP 0b11100010
#define LOOPZ 0b11100001
#define LOOPNZ 0b11100000
#define JCXZ 0b11100011

void InitOpTable(inst_t *op_table)
{
    op_table[MOV_RM_TO_FROM_REG].operation_type = MOV;
    op_table[MOV_RM_TO_FROM_REG].field[D] = (inst_field_t){D, UNINITIALIZED, 1, 0b1, 0};
    op_table[MOV_RM_TO_FROM_REG].field[W] = (inst_field_t){W, UNINITIALIZED, 0, 1, 0};
    op_table[MOV_RM_TO_FROM_REG].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0};
    op_table[MOV_RM_TO_FROM_REG].field[MOD] = (inst_field_t){MOD, UNINITIALIZED, 14, 0b11, 0};
    op_table[MOV_RM_TO_FROM_REG].field[REG] = (inst_field_t){REG, UNINITIALIZED, 11, 0b111, 0};
    op_table[MOV_RM_TO_FROM_REG].field[RM] = (inst_field_t){RM, UNINITIALIZED, 8, 0b111, 0};
    op_table[MOV_RM_TO_FROM_REG].field[DISP] = (inst_field_t){DISP, UNINITIALIZED, 0, 0, 0};
    op_table[MOV_RM_TO_FROM_REG].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[MOV_RM_TO_FROM_REG].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, NOT_USED, 0, 0, 0};

    op_table[MOV_IMMEDIATE_TO_RM].operation_type = MOV;
    op_table[MOV_IMMEDIATE_TO_RM].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_RM].field[W] = (inst_field_t){W, UNINITIALIZED, 0, 1, 0};
    op_table[MOV_IMMEDIATE_TO_RM].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_RM].field[MOD] = (inst_field_t){MOD, UNINITIALIZED, 14, 0b11, 0};
    op_table[MOV_IMMEDIATE_TO_RM].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_RM].field[RM] = (inst_field_t){RM, UNINITIALIZED, 8, 0b111, 0};
    op_table[MOV_IMMEDIATE_TO_RM].field[DISP] = (inst_field_t){DISP, UNINITIALIZED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_RM].field[DATA] = (inst_field_t){DATA, UNINITIALIZED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_RM].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, NOT_USED, 0, 0, 0};

    op_table[MOV_IMMEDIATE_TO_REG].operation_type = MOV;
    op_table[MOV_IMMEDIATE_TO_REG].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[MOV_IMMEDIATE_TO_REG].field[W] = (inst_field_t){W, INITIALIZED, 3, 0b1, 0}; 
    op_table[MOV_IMMEDIATE_TO_REG].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[MOV_IMMEDIATE_TO_REG].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_REG].field[REG] = (inst_field_t){REG, UNINITIALIZED, 0, 0b111, 0};
    op_table[MOV_IMMEDIATE_TO_REG].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_REG].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_REG].field[DATA] = (inst_field_t){DATA, UNINITIALIZED, 8, 0xff, 0};
    op_table[MOV_IMMEDIATE_TO_REG].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, NOT_USED, 0, 0, 0};

    op_table[MOV_IMMEDIATE_TO_REG_W].operation_type = MOV;
    op_table[MOV_IMMEDIATE_TO_REG_W].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[MOV_IMMEDIATE_TO_REG_W].field[W] = (inst_field_t){W, INITIALIZED, 3, 0b1, 1}; 
    op_table[MOV_IMMEDIATE_TO_REG_W].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[MOV_IMMEDIATE_TO_REG_W].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_REG_W].field[REG] = (inst_field_t){REG, UNINITIALIZED, 0, 0b111, 0};
    op_table[MOV_IMMEDIATE_TO_REG_W].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_REG_W].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_REG_W].field[DATA] = (inst_field_t){DATA, UNINITIALIZED, 8, 0xff, 0};
    op_table[MOV_IMMEDIATE_TO_REG_W].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, NOT_USED, 0, 0, 0};

    op_table[MOV_IMMEDIATE_TO_REG_REG].operation_type = MOV;
    op_table[MOV_IMMEDIATE_TO_REG_REG].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[MOV_IMMEDIATE_TO_REG_REG].field[W] = (inst_field_t){W, INITIALIZED, 3, 0b1, 0}; 
    op_table[MOV_IMMEDIATE_TO_REG_REG].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[MOV_IMMEDIATE_TO_REG_REG].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_REG_REG].field[REG] = (inst_field_t){REG, UNINITIALIZED, 0, 0b111, 0};
    op_table[MOV_IMMEDIATE_TO_REG_REG].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_REG_REG].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_REG_REG].field[DATA] = (inst_field_t){DATA, UNINITIALIZED, 8, 0xff, 0};
    op_table[MOV_IMMEDIATE_TO_REG_REG].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, NOT_USED, 0, 0, 0};

    op_table[MOV_IMMEDIATE_TO_REG_W_REG].operation_type = MOV;
    op_table[MOV_IMMEDIATE_TO_REG_W_REG].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[MOV_IMMEDIATE_TO_REG_W_REG].field[W] = (inst_field_t){W, INITIALIZED, 3, 0b1, 1}; 
    op_table[MOV_IMMEDIATE_TO_REG_W_REG].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[MOV_IMMEDIATE_TO_REG_W_REG].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_REG_W_REG].field[REG] = (inst_field_t){REG, UNINITIALIZED, 0, 0b111, 0};
    op_table[MOV_IMMEDIATE_TO_REG_W_REG].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_REG_W_REG].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_REG_W_REG].field[DATA] = (inst_field_t){DATA, UNINITIALIZED, 0, 0, 0};
    op_table[MOV_IMMEDIATE_TO_REG_W_REG].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, NOT_USED, 0, 0, 0};

    op_table[ADD_SUB_CMP_IMMEDIATE_TO_RM].operation_type = NONE;
    op_table[ADD_SUB_CMP_IMMEDIATE_TO_RM].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[ADD_SUB_CMP_IMMEDIATE_TO_RM].field[W] = (inst_field_t){W, UNINITIALIZED, 0, 0b1, 0}; 
    op_table[ADD_SUB_CMP_IMMEDIATE_TO_RM].field[S] = (inst_field_t){S, UNINITIALIZED, 1, 0b1, 1}; 
    op_table[ADD_SUB_CMP_IMMEDIATE_TO_RM].field[MOD] = (inst_field_t){MOD, UNINITIALIZED, 14, 0b11, 0};
    op_table[ADD_SUB_CMP_IMMEDIATE_TO_RM].field[REG] = (inst_field_t){REG, NOT_USED, 11, 0b111, 0};
    op_table[ADD_SUB_CMP_IMMEDIATE_TO_RM].field[RM] = (inst_field_t){RM, UNINITIALIZED, 8, 0b111, 0};
    op_table[ADD_SUB_CMP_IMMEDIATE_TO_RM].field[DISP] = (inst_field_t){DISP, UNINITIALIZED, 0, 0, 0};
    op_table[ADD_SUB_CMP_IMMEDIATE_TO_RM].field[DATA] = (inst_field_t){DATA, UNINITIALIZED, 0, 0, 0};
    op_table[ADD_SUB_CMP_IMMEDIATE_TO_RM].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, NOT_USED, 0, 0, 0};

    op_table[ADD_RM_TO_FROM_REG].operation_type = ADD;
    op_table[ADD_RM_TO_FROM_REG].field[D] = (inst_field_t){D, UNINITIALIZED, 1, 0b1, 0}; 
    op_table[ADD_RM_TO_FROM_REG].field[W] = (inst_field_t){W, UNINITIALIZED, 0, 0b1, 0}; 
    op_table[ADD_RM_TO_FROM_REG].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[ADD_RM_TO_FROM_REG].field[MOD] = (inst_field_t){MOD, UNINITIALIZED, 14, 0b11, 0};
    op_table[ADD_RM_TO_FROM_REG].field[REG] = (inst_field_t){REG, UNINITIALIZED, 11, 0b111, 0};
    op_table[ADD_RM_TO_FROM_REG].field[RM] = (inst_field_t){RM, UNINITIALIZED, 8, 0b111, 0};
    op_table[ADD_RM_TO_FROM_REG].field[DISP] = (inst_field_t){DISP, UNINITIALIZED, 0, 0, 0};
    op_table[ADD_RM_TO_FROM_REG].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[ADD_RM_TO_FROM_REG].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, NOT_USED, 0, 0, 0};

    op_table[ADD_IMMEDIATE_TO_ACC].operation_type = ADD;
    op_table[ADD_IMMEDIATE_TO_ACC].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[ADD_IMMEDIATE_TO_ACC].field[W] = (inst_field_t){W, UNINITIALIZED, 0, 0b1, 0}; 
    op_table[ADD_IMMEDIATE_TO_ACC].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[ADD_IMMEDIATE_TO_ACC].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[ADD_IMMEDIATE_TO_ACC].field[REG] = (inst_field_t){REG, INITIALIZED, 0, 0, 0}; // NOTE: changed to INITIALIZED
    op_table[ADD_IMMEDIATE_TO_ACC].field[RM] = (inst_field_t){RM, NOT_USED, 8, 0b111, 0};
    op_table[ADD_IMMEDIATE_TO_ACC].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[ADD_IMMEDIATE_TO_ACC].field[DATA] = (inst_field_t){DATA, UNINITIALIZED, 8, 0b11111111, 0};
    op_table[ADD_IMMEDIATE_TO_ACC].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, NOT_USED, 0, 0, 0};

    op_table[SUB_RM_TO_FROM_REG].operation_type = SUB;
    op_table[SUB_RM_TO_FROM_REG].field[D] = (inst_field_t){D, UNINITIALIZED, 1, 0b1, 0}; 
    op_table[SUB_RM_TO_FROM_REG].field[W] = (inst_field_t){W, UNINITIALIZED, 0, 0b1, 0}; 
    op_table[SUB_RM_TO_FROM_REG].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[SUB_RM_TO_FROM_REG].field[MOD] = (inst_field_t){MOD, UNINITIALIZED, 14, 0b11, 0};
    op_table[SUB_RM_TO_FROM_REG].field[REG] = (inst_field_t){REG, UNINITIALIZED, 11, 0b111, 0};
    op_table[SUB_RM_TO_FROM_REG].field[RM] = (inst_field_t){RM, UNINITIALIZED, 8, 0b111, 0};
    op_table[SUB_RM_TO_FROM_REG].field[DISP] = (inst_field_t){DISP, UNINITIALIZED, 0, 0, 0};
    op_table[SUB_RM_TO_FROM_REG].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[SUB_RM_TO_FROM_REG].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, NOT_USED, 0, 0, 0};

    op_table[SUB_IMMEDIATE_TO_ACC].operation_type = SUB;
    op_table[SUB_IMMEDIATE_TO_ACC].field[D] = (inst_field_t){D, NOT_USED, 1, 0b1, 0}; 
    op_table[SUB_IMMEDIATE_TO_ACC].field[W] = (inst_field_t){W, UNINITIALIZED, 0, 0b1, 0}; 
    op_table[SUB_IMMEDIATE_TO_ACC].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[SUB_IMMEDIATE_TO_ACC].field[MOD] = (inst_field_t){MOD, NOT_USED, 14, 0b11, 0};
    op_table[SUB_IMMEDIATE_TO_ACC].field[REG] = (inst_field_t){REG, NOT_USED, 11, 0b111, 0};
    op_table[SUB_IMMEDIATE_TO_ACC].field[RM] = (inst_field_t){RM, NOT_USED, 8, 0b111, 0};
    op_table[SUB_IMMEDIATE_TO_ACC].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[SUB_IMMEDIATE_TO_ACC].field[DATA] = (inst_field_t){DATA, UNINITIALIZED, 8, 0b11111111, 0};
    op_table[SUB_IMMEDIATE_TO_ACC].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, NOT_USED, 0, 0, 0};

    op_table[CMP_RM_TO_FROM_REG].operation_type = CMP;
    op_table[CMP_RM_TO_FROM_REG].field[D] = (inst_field_t){D, UNINITIALIZED, 1, 0b1, 0}; 
    op_table[CMP_RM_TO_FROM_REG].field[W] = (inst_field_t){W, UNINITIALIZED, 0, 0b1, 0}; 
    op_table[CMP_RM_TO_FROM_REG].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[CMP_RM_TO_FROM_REG].field[MOD] = (inst_field_t){MOD, UNINITIALIZED, 14, 0b11, 0};
    op_table[CMP_RM_TO_FROM_REG].field[REG] = (inst_field_t){REG, UNINITIALIZED, 11, 0b111, 0};
    op_table[CMP_RM_TO_FROM_REG].field[RM] = (inst_field_t){RM, UNINITIALIZED, 8, 0b111, 0};
    op_table[CMP_RM_TO_FROM_REG].field[DISP] = (inst_field_t){DISP, UNINITIALIZED, 0, 0, 0};
    op_table[CMP_RM_TO_FROM_REG].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[CMP_RM_TO_FROM_REG].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, NOT_USED, 0, 0, 0};

    op_table[CMP_IMMEDIATE_TO_ACC].operation_type = CMP;
    op_table[CMP_IMMEDIATE_TO_ACC].field[D] = (inst_field_t){D, NOT_USED, 1, 0b1, 0}; 
    op_table[CMP_IMMEDIATE_TO_ACC].field[W] = (inst_field_t){W, UNINITIALIZED, 0, 0b1, 0}; 
    op_table[CMP_IMMEDIATE_TO_ACC].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[CMP_IMMEDIATE_TO_ACC].field[MOD] = (inst_field_t){MOD, NOT_USED, 14, 0b11, 0};
    op_table[CMP_IMMEDIATE_TO_ACC].field[REG] = (inst_field_t){REG, NOT_USED, 11, 0b111, 0};
    op_table[CMP_IMMEDIATE_TO_ACC].field[RM] = (inst_field_t){RM, NOT_USED, 8, 0b111, 0};
    op_table[CMP_IMMEDIATE_TO_ACC].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[CMP_IMMEDIATE_TO_ACC].field[DATA] = (inst_field_t){DATA, UNINITIALIZED, 8, 0b11111111, 0};
    op_table[CMP_IMMEDIATE_TO_ACC].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, NOT_USED, 0, 0, 0};

    op_table[JE].operation_type = JMP;
    op_table[JE].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[JE].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[JE].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[JE].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[JE].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[JE].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[JE].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[JE].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[JE].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[JL].operation_type = JMP;
    op_table[JL].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[JL].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[JL].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[JL].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[JL].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[JL].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[JL].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[JL].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[JL].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[JLE].operation_type = JMP;
    op_table[JLE].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[JLE].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[JLE].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[JLE].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[JLE].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[JLE].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[JLE].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[JLE].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[JLE].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[JB].operation_type = JMP;
    op_table[JB].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[JB].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[JB].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[JB].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[JB].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[JB].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[JB].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[JB].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[JB].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[JBE].operation_type = JMP;
    op_table[JBE].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[JBE].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[JBE].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[JBE].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[JBE].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[JBE].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[JBE].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[JBE].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[JBE].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[JP].operation_type = JMP;
    op_table[JP].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[JP].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[JP].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[JP].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[JP].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[JP].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[JP].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[JP].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[JP].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[JO].operation_type = JMP;
    op_table[JO].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[JO].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[JO].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[JO].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[JO].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[JO].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[JO].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[JO].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[JO].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[JS].operation_type = JMP;
    op_table[JS].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[JS].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[JS].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[JS].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[JS].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[JS].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[JS].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[JS].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[JS].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[JNE].operation_type = JMP;
    op_table[JNE].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[JNE].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[JNE].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[JNE].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[JNE].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[JNE].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[JNE].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[JNE].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[JNE].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[JNL].operation_type = JMP;
    op_table[JNL].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[JNL].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[JNL].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[JNL].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[JNL].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[JNL].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[JNL].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[JNL].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[JNL].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[JG].operation_type = JMP;
    op_table[JG].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[JG].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[JG].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[JG].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[JG].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[JG].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[JG].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[JG].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[JG].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[JNB].operation_type = JMP;
    op_table[JNB].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[JNB].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[JNB].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[JNB].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[JNB].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[JNB].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[JNB].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[JNB].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[JNB].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[JA].operation_type = JMP;
    op_table[JA].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[JA].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[JA].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[JA].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[JA].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[JA].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[JA].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[JA].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[JA].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[JNP].operation_type = JMP;
    op_table[JNP].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[JNP].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[JNP].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[JNP].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[JNP].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[JNP].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[JNP].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[JNP].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[JNP].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[JNO].operation_type = JMP;
    op_table[JNO].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[JNO].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[JNO].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[JNO].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[JNO].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[JNO].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[JNO].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[JNO].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[JNO].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[JNS].operation_type = JMP;
    op_table[JNS].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[JNS].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[JNS].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[JNS].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[JNS].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[JNS].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[JNS].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[JNS].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[JNS].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[LOOP].operation_type = JMP;
    op_table[LOOP].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[LOOP].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[LOOP].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[LOOP].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[LOOP].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[LOOP].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[LOOP].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[LOOP].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[LOOP].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[LOOPZ].operation_type = JMP;
    op_table[LOOPZ].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[LOOPZ].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[LOOPZ].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[LOOPZ].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[LOOPZ].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[LOOPZ].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[LOOPZ].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[LOOPZ].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[LOOPZ].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[LOOPNZ].operation_type = JMP;
    op_table[LOOPNZ].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[LOOPNZ].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[LOOPNZ].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[LOOPNZ].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[LOOPNZ].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[LOOPNZ].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[LOOPNZ].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[LOOPNZ].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[LOOPNZ].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};

    op_table[JCXZ].operation_type = JMP;
    op_table[JCXZ].field[D] = (inst_field_t){D, NOT_USED, 0, 0, 0}; 
    op_table[JCXZ].field[W] = (inst_field_t){W, NOT_USED, 0, 0, 0}; 
    op_table[JCXZ].field[S] = (inst_field_t){S, NOT_USED, 0, 0, 0}; 
    op_table[JCXZ].field[MOD] = (inst_field_t){MOD, NOT_USED, 0, 0, 0};
    op_table[JCXZ].field[REG] = (inst_field_t){REG, NOT_USED, 0, 0, 0};
    op_table[JCXZ].field[RM] = (inst_field_t){RM, NOT_USED, 0, 0, 0};
    op_table[JCXZ].field[DISP] = (inst_field_t){DISP, NOT_USED, 0, 0, 0};
    op_table[JCXZ].field[DATA] = (inst_field_t){DATA, NOT_USED, 0, 0, 0};
    op_table[JCXZ].field[JMP_OFFSET] = (inst_field_t){JMP_OFFSET, UNINITIALIZED, 8, 0b11111111, 0};
}