#include "sim86_decoder.h"
#include "sim86_instruction.h"
#include "sim86_clock_estimator.h"

static clock_est_t clock_estimation_table[NUM_OPERATION_TYPES][NUM_ACCESS_TYPES - 1][NUM_ACCESS_TYPES] = 
{
    {
        {NONE,NONE,NONE,NONE},
        {NONE,NONE,NONE,NONE},
        {NONE,NONE,NONE,NONE}
    },
    {// MOV

        {// DEST REG_ACCESS
            {2, 0},                 // SRC REG_ACCESS
            {NONE, NONE},           // SRC ACC_ACCESS
            {8, UNINITIALIZED},     // SRC MEM_ACCESS
            {4, 0}                  // SRC IMMEDIATE_ACCESS
        }, 
        {// DEST ACC_ACCESS
            {NONE, NONE},           // SRC REG_ACCESS
            {NONE, NONE},           // SRC ACC_ACCESS
            {10, 0},                // SRC MEM_ACCESS
            {NONE, NONE}            // SRC IMMEDIATE_ACCESS
        },
        {// DEST MEM_ACCESS
            {9, UNINITIALIZED},     // SRC REG_ACCESS
            {10, 0},                // SRC ACC_ACCESS
            {NONE, NONE},           // SRC MEM_ACCESS
            {10, UNINITIALIZED}     // SRC IMMEDIATE_ACCESS
        }
    },
    {// ADD

        {// DEST REG_ACCESS
            {3, 0},                 // SRC REG_ACCESS
            {NONE, NONE},           // SRC ACC_ACCESS
            {9, UNINITIALIZED},     // SRC MEM_ACCESS 
            {4, 0}                  // SRC IMMEDIATE_ACCESS
        }, 
        {// DEST ACC_ACCESS
            {NONE, NONE},           // SRC REG_ACCESS
            {NONE, NONE},           // SRC ACC_ACCESS
            {NONE, NONE},           // SRC MEM_ACCESS
            {4, 0}                  // SRC IMMEDIATE_ACCESS
        },
        {// DEST MEM_ACCESS
            {16, UNINITIALIZED},    // SRC REG_ACCESS
            {NONE, NONE},           // SRC ACC_ACCESS
            {NONE, NONE},           // SRC MEM_ACCESS
            {17, UNINITIALIZED}     // SRC IMMEDIATE_ACCESS
        }
    },
    {// SUB

        {// DEST REG_ACCESS
            {3, 0},                 // SRC REG_ACCESS
            {NONE, NONE},           // SRC ACC_ACCESS
            {9, UNINITIALIZED},     // SRC MEM_ACCESS
            {4, 0}                  // SRC IMMEDIATE_ACCESS
        }, 
        {// DEST ACC_ACCESS
            {NONE, NONE},           // SRC REG_ACCESS
            {NONE, NONE},           // SRC ACC_ACCESS
            {NONE, NONE},           // SRC MEM_ACCESS
            {4, 0}                  // SRC IMMEDIATE_ACCESS
        },
        {// DEST MEM_ACCESS
            {9, UNINITIALIZED},     // SRC REG_ACCESS
            {NONE, NONE},           // SRC ACC_ACCESS
            {NONE, NONE},           // SRC MEM_ACCESS
            {10, UNINITIALIZED}     // SRC IMMEDIATE_ACCESS
        }
    },
    {// CMP

        {// DEST REG_ACCESS
            {3, 0},                 // SRC REG_ACCESS
            {NONE, NONE},           // SRC ACC_ACCESS
            {9, UNINITIALIZED},     // SRC MEM_ACCESS
            {4, 0}                  // SRC IMMEDIATE_ACCESS
        }, 
        {// DEST ACC_ACCESS
            {NONE, NONE},           // SRC REG_ACCESS
            {NONE, NONE},           // SRC ACC_ACCESS
            {NONE, NONE},           // SRC MEM_ACCESS
            {4, 0}                  // SRC IMMEDIATE_ACCESS
        },
        {// DEST MEM_ACCESS
            {9, UNINITIALIZED},    // SRC REG_ACCESS
            {NONE, NONE},          // SRC ACC_ACCESS
            {NONE, NONE},          // SRC MEM_ACCESS
            {10, UNINITIALIZED}    // SRC IMMEDIATE_ACCESS
        }
    },
    {// JMP

        {// DEST REG_ACCESS
            {NONE, NONE},   // SRC REG_ACCESS
            {NONE, NONE},   // SRC ACC_ACCESS
            {NONE, NONE},   // SRC MEM_ACCESS
            {4, NONE}       // SRC IMMEDIATE_ACCESS
        }, 
        {// DEST ACC_ACCESS
            {NONE, NONE},   // SRC REG_ACCESS
            {NONE, NONE},   // SRC ACC_ACCESS
            {NONE, NONE},   // SRC MEM_ACCESS
            {NONE, NONE}    // SRC IMMEDIATE_ACCESS
        },
        {// DEST MEM_ACCESS
            {NONE, NONE},   // SRC REG_ACCESS
            {NONE, NONE},   // SRC ACC_ACCESS
            {NONE, NONE},   // SRC MEM_ACCESS
            {NONE, NONE}    // SRC IMMEDIATE_ACCESS
        }
    }
};

static enum access_type operand_type_access_type_trans_table[NUM_OPERAND_TYPES] = 
{
    REG_ACCESS, MEM_ACCESS, MEM_ACCESS, IMMEDIATE_ACCESS, IMMEDIATE_ACCESS, IMMEDIATE_ACCESS // NOTE(16.7.24): not handling accumulator access
};

void EstimatorGetClocks(expression_t *decoded_inst)
{
    enum access_type dest_access = operand_type_access_type_trans_table[decoded_inst->operands[DEST].operand_type];
    enum access_type src_access = operand_type_access_type_trans_table[decoded_inst->operands[SRC].operand_type];
    enum operation_type op_type = decoded_inst->operation_type;

    decoded_inst->clock_estimation = clock_estimation_table[op_type][dest_access][src_access];

    // TODO(16.7.24): turn this if into a table
    if (decoded_inst->clock_estimation.ea_clocks == UNINITIALIZED)
    {
        operand_t *mem_operand = dest_access == MEM_ACCESS ? &decoded_inst->operands[DEST] : &decoded_inst->operands[SRC];

        if (mem_operand->operand_type == DIRECT_ADDR)
        {
            decoded_inst->clock_estimation.ea_clocks = 6;
        }
        else if (mem_operand->ea_code >= 4)
        {
            if (mem_operand->disp == 0)
            {
                decoded_inst->clock_estimation.ea_clocks = 5;
            }
            else
            {
                decoded_inst->clock_estimation.ea_clocks = 9;
            }
        }
        else if (mem_operand->ea_code == 0 || mem_operand->ea_code == 3)
        {
            if (mem_operand->disp == 0)
            {
                decoded_inst->clock_estimation.ea_clocks = 7;
            }
            else
            {
                decoded_inst->clock_estimation.ea_clocks = 11;
            }
        }
        else if (mem_operand->ea_code == 2 || mem_operand->ea_code == 1)
        {
            if (mem_operand->disp == 0)
            {
                decoded_inst->clock_estimation.ea_clocks = 8;
            }
            else
            {
                decoded_inst->clock_estimation.ea_clocks = 12;
            }
        }
    }
}