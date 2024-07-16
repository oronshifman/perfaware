#ifndef __SIM86_CLOCK_ESTIMATOR_H__
#define __SIM86_CLOCK_ESTIMATOR_H__

#include "sim86_decoder.h"
#include "my_int.h"

enum access_type
{
    REG_ACCESS,
    ACC_ACCESS,
    MEM_ACCESS,
    IMMEDIATE_ACCESS,

    NUM_ACCESS_TYPES
};

typedef struct clock_estimation
{
    u8 base_clocks;
    u8 ea_clocks;
} clock_est_t;

void EstimatorGetClocks(expression_t *decoded_inst);

#endif /* SIM86_CLOCK_ESTIMATOR_H */
