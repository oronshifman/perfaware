/* ------------------------------------------*/ 
/* Filename: haversine_generator.cpp         */
/* Date:     17.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include <vector>
#include <fstream> // ifstream
#include <iostream>
#include <string>

#include "haversine_generator.h"
#include "listing_0065_haversine_formula.h"
#include "my_int.h"

#define NUM_REGIONS 16

#define X_LOW_BOUND -180
#define X_UP_BOUND 180
#define Y_LOW_BOUND -90
#define Y_UP_BOUND 90

#define REGION_RADIOS 45

#define EARTH_RADIUS 6372.8

Point region_centers[NUM_REGIONS] = 
{
    {-135, -135}, {-135,-45}, {-135, 45}, {-135, 135},
    {-45, -135}, {-45,-45}, {-45, 45}, {-45, 135},
    {45, -135}, {45,-45}, {45, 45}, {45, 135},
    {135, -135}, {135,-45}, {135, 45}, {135, 135},
};

static f64 GetRandX(u8 stab);
static f64 GetRandY(u8 stab);
static f64 GetRandYForRegion(u8 region_num);
static f64 GetRandXForRegion(u8 region_num);

typedef f64 (*random_x_generator)(u8);
typedef f64 (*random_y_generator)(u8);

b8 HaversineJSONGenerator::GeneratePoints(exec_option exec_option, const u64 seed, const u64 num_points)
{
    srand(seed);

    random_x_generator rand_x_gen;
    random_y_generator rand_y_gen;
    std::string method;
    if (exec_option == UNIFORM)
    {
        rand_x_gen = GetRandX;
        rand_y_gen = GetRandY;
        method = "uniform";
    }
    else
    {
        rand_x_gen = GetRandYForRegion;
        rand_y_gen = GetRandXForRegion;
        method = "cluster";
    }

    std::string file_name_prefix = method + "_" + std::to_string(seed) + "_" + std::to_string(num_points);
    
    std::string json_dir = "haversine_jsons/";
    std::string json_name = file_name_prefix + "_points.json";

    std::string bin_dir = "haversine_distances_bins/";
    std::string distances_name =  file_name_prefix + "_distances.bin";

    std::ofstream json_file;
    json_file.open(json_dir + json_name);
    if (!json_file.good())
    {
        std::cerr << "Couldn't open " << json_name << std::endl;
        perror("json_file: ");
        return 0;
    }
    std::ofstream distances_bin;
    distances_bin.open(bin_dir + distances_name);
    if (!distances_bin.good())
    {
        std::cerr << "Couldn't open " << distances_name << std::endl;
        perror("distances_bin: ");
        return 0;
    }
    
    f64 sum_coef = 1.0 / (f64)num_points;

    if (!json_file.good() || !distances_bin.good())
    {
    }

    json_file << "{\"pairs\":[\n";
    for (u64 point = 0; point < num_points; ++point)
    {
        HaverPair new_pair;

        new_pair.x0 = rand_x_gen(point % NUM_REGIONS);
        new_pair.y0 = rand_y_gen(point % NUM_REGIONS);
        new_pair.x1 = rand_x_gen(point % NUM_REGIONS);
        new_pair.y1 = rand_y_gen(point % NUM_REGIONS);

        json_file << "{\"x0\":" + std::to_string(new_pair.x0) + "," +
                     "\"y0\":" + std::to_string(new_pair.y0) + "," +
                     "\"x1\":" + std::to_string(new_pair.x1) + "," +
                     "\"y1\":" + std::to_string(new_pair.y1) + "}";
        json_file << (point == num_points - 1 ? "\n" : ",\n");
        
        f64 distance = ReferenceHaversine(new_pair.x0, new_pair.y0, new_pair.x1, new_pair.y1, EARTH_RADIUS);
        distances_bin.write(reinterpret_cast<s8*>(&distance), sizeof(distance));

        expected_sum += distance * sum_coef;
    }

    json_file << "]}";

    json_file.close();
    distances_bin.close();

    return 1;
}

static f64 GetRandX(u8 stab)
{
    return X_LOW_BOUND + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (X_UP_BOUND - X_LOW_BOUND)));
}

static f64 GetRandY(u8 stab)
{
    return Y_LOW_BOUND + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Y_UP_BOUND - Y_LOW_BOUND)));
}

static f64 GetRandXForRegion(u8 region_num)
{
    s16 region_low = region_centers[region_num].x - REGION_RADIOS;
    s16 region_high = region_centers[region_num].x + REGION_RADIOS;
    return region_low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (region_high - region_low)));
}

static f64 GetRandYForRegion(u8 region_num)
{
    s16 region_low = region_centers[region_num].y - REGION_RADIOS;
    s16 region_high = region_centers[region_num].y + REGION_RADIOS;
    return region_low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (region_high - region_low)));
}
