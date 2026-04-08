/* ---------------------------------*/ 
/* Filename: haversine_calc.cpp     */
/* Date:     25.09.2024             */
/* Author:   Oron                   */ 
/* ---------------------------------*/

#include <iostream>
#include <sys/stat.h>
#include <fstream>

#include "listing_0065_haversine_formula.h"
#include "haversine_calc.h"
#include "JSONObject.h"
#include "profiler.h"

using namespace JSORON;

#define EARTH_RADIUS 6372.8

f64 HaversineCalc(const JSONObject& json)
{
    Profiler_TimeBandwidth(__func__, ((JSONArray&)json.At("pairs")).Size() * 32); // NOTE(25.09.24): PROFILING

    const JSONArray& pairs = json.At("pairs");
    u64 num_points = pairs.Size();
    f32 sum_coef = 1.0 / (f32)num_points;

    f64 sum = 0.0;
    
    for (auto& pair : pairs)
    {

        f64 x0 = pair.At("x0");
        f64 y0 = pair.At("y0");
        f64 x1 = pair.At("x1");
        f64 y1 = pair.At("y1");

        f32 distance = ReferenceHaversine(x0, y0, x1, y1, EARTH_RADIUS);
        sum += distance * sum_coef;
    }

    return sum;
}

char *ReadEntireFile(std::ifstream& in, const std::string& filename)
{
    Profiler_TimeFunction; // NOTE(25.09.24): PROFILING

	u64 file_size = GetFileSize(filename);

	char *data = (char *)malloc(file_size + 1);

    {
    Profiler_TimeBandwidth("read", file_size); // NOTE(29.10.24): PROFILING
    in.read(data, file_size);
	if (in.fail())
    {
        std::cerr << "ERROR - Failed to read file: " << filename << "\n"; 
	    return nullptr;
    }
    }
    return data;
}

u64 GetFileSize(std::ifstream& in)
{
	u64 file_size = 0;
	in.seekg(0, std::ios_base::end);
	file_size = in.tellg();
	in.seekg(0);

    return file_size;
}

u64 GetFileSize(const std::string& filename)
{
	struct stat file_stat;
    stat(filename.c_str(), &file_stat);

    return file_stat.st_size;
}
