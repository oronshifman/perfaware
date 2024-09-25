/* ------------------------------------------*/ 
/* Filename: haversine_generator.h           */
/* Date:     17.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#ifndef __HAVERSINE_GENERATOR_H__
#define __HAVERSINE_GENERATOR_H__

#include <vector>

#include "main.h"
#include "my_int.h"

struct Point
{
    f64 x;
    f64 y;
};

struct HaverPair 
{
    f64 x0;
    f64 y0;

    f64 x1;
    f64 y1;
};

class HaversineJSONGenerator 
{
public:
    b8 GeneratePoints(exec_option exec_option, const u64 seed, const u64 num_points);

    f64 expected_sum;
};

#endif /* HAVERSINE_GENERATOR_H */
