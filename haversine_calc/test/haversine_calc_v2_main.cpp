/* -------------------------------------------*/ 
/* Filename: haversine_calc_profiled_main.cpp */
/* Date:     23.09.2024                       */
/* Author:   Oron                             */ 
/* -------------------------------------------*/

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <iomanip> // std::setprecision
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <sys/stat.h>

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int32_t b32;

typedef float f32;
typedef double f64;

struct haversine_pair
{
    f64 X0, Y0;
    f64 X1, Y1;
};

struct profiling_data 
{
	// In CPU clocks
	u64 startup;
	u64 read;
	u64 misc_setup;
	u64 parse;
	u64 sum;
	u64 misc_output; 

	// In OS Timer clocks
	u64 total_time;

	u64 cpu_freq;
	u64 os_freq;
};

profiling_data pd = {0};

#include "../../casey_code/listing_0065_haversine_formula.cpp"
#include "../../casey_code/listing_0068_buffer.cpp"
#include "../casey_code/listing_0069_lookup_json_parser.cpp"
#include "profiler.h"

static buffer ReadEntireFile(char *FileName)
{
    buffer Result = {};
        
    FILE *File = fopen(FileName, "rb");
    if(File)
    {
#if _WIN32
        struct __stat64 Stat;
        _stat64(FileName, &Stat);
#else
        struct stat Stat;
        stat(FileName, &Stat);
#endif
        
        Result = AllocateBuffer(Stat.st_size);
        if(Result.Data)
        {
            if(fread(Result.Data, Result.Count, 1, File) != 1)
            {
                fprintf(stderr, "ERROR: Unable to read \"%s\".\n", FileName);
                FreeBuffer(&Result);
            }
        }
        
        fclose(File);
    }
    else
    {
        fprintf(stderr, "ERROR: Unable to open \"%s\".\n", FileName);
    }
    
    return Result;
}

static f64 SumHaversineDistances(u64 PairCount, haversine_pair *Pairs)
{
    f64 Sum = 0;
    
    f64 SumCoef = 1 / (f64)PairCount;
    for(u64 PairIndex = 0; PairIndex < PairCount; ++PairIndex)
    {
        haversine_pair Pair = Pairs[PairIndex];
        f64 EarthRadius = 6372.8;
        f64 Dist = ReferenceHaversine(Pair.X0, Pair.Y0, Pair.X1, Pair.Y1, EarthRadius);
        Sum += SumCoef*Dist;
    }
    
    return Sum;
}

void OutputProfilingStats()
{
	std::cout << "\n\n";

	u64 total_cpu = pd.startup;
	total_cpu += pd.read;
	total_cpu += pd.misc_setup;
	total_cpu += pd.parse;
	total_cpu += pd.sum;
	total_cpu += pd.misc_output;

	std::cout << std::fixed << std::setprecision(8);
	std::cout << "Total time: " << (f64)pd.total_time / 1000000 << "ms (CPU freq " << pd.cpu_freq << ")\n\n";
	
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "       Startup: " << pd.startup << " (" << (f64)pd.startup / total_cpu * 100 << "%)\n"
			  << "          Read: " << pd.read << " (" <<  (f64)pd.read / total_cpu * 100 << "%)\n"
			  << "     MiscSetup: " << pd.misc_setup << " (" <<  (f64)pd.misc_setup / total_cpu * 100<< "%)\n"
			  << "         Parse: " << pd.parse << " (" <<  (f64)pd.parse / total_cpu * 100 << "%)\n"
			  << "           Sum: " << pd.sum << " (" <<  (f64)pd.sum / total_cpu * 100 << "%)\n"
			  << "    MiscOutput: " << pd.misc_output << " (" <<  (f64)pd.misc_output / total_cpu * 100 << "%)\n\n";
}

int main(int ArgCount, char **Args)
{
pd.cpu_freq = profiler::GetCPUFreq(100);
pd.os_freq = profiler::GetOSTimerFreq();
u64 start = 0;

pd.total_time = profiler::StartOS();

start = profiler::StartCPU();
    int Result = 1;
    
    if((ArgCount == 2) || (ArgCount == 3))
    {
pd.startup = profiler::EndCPU(start);	

start = profiler::StartCPU();
        buffer InputJSON = ReadEntireFile(Args[1]);
pd.read = profiler::EndCPU(start);	
        
start = profiler::StartCPU();
        u32 MinimumJSONPairEncoding = 6*4;
        u64 MaxPairCount = InputJSON.Count / MinimumJSONPairEncoding;
        if(MaxPairCount)
        {
            buffer ParsedValues = AllocateBuffer(MaxPairCount * sizeof(haversine_pair));
            if(ParsedValues.Count)
            {
                haversine_pair *Pairs = (haversine_pair *)ParsedValues.Data;
pd.misc_setup = profiler::EndCPU(start);

start = profiler::StartCPU();
                u64 PairCount = ParseHaversinePairs(InputJSON, MaxPairCount, Pairs);
pd.parse = profiler::EndCPU(start);

start = profiler::StartCPU();
                f64 Sum = SumHaversineDistances(PairCount, Pairs);
pd.sum = profiler::EndCPU(start);
                
start = profiler::StartCPU();
                fprintf(stdout, "Input size: %lu\n", InputJSON.Count);
                fprintf(stdout, "Pair count: %lu\n", PairCount);
                fprintf(stdout, "Haversine sum: %.16f\n", Sum);
pd.misc_output = profiler::EndCPU(start);
                
                if(ArgCount == 3)
                {
start = profiler::StartCPU();
                    buffer AnswersF64 = ReadEntireFile(Args[2]);
pd.read = profiler::EndCPU(start);
                    if(AnswersF64.Count >= sizeof(f64))
                    {
start = profiler::StartCPU();
                        f64 *AnswerValues = (f64 *)AnswersF64.Data;
pd.misc_setup = profiler::EndCPU(start);
                        
start = profiler::StartCPU();
                        fprintf(stdout, "\nValidation:\n");
pd.misc_output = profiler::EndCPU(start);
                        
start = profiler::StartCPU();
                        u64 RefAnswerCount = (AnswersF64.Count - sizeof(f64)) / sizeof(f64);
pd.misc_setup = profiler::EndCPU(start);
                        if(PairCount != RefAnswerCount)
                        {
                            fprintf(stdout, "FAILED - pair count doesn't match %lu.\n", RefAnswerCount);
                        }
                        
start = profiler::StartCPU();
                        f64 RefSum = AnswerValues[RefAnswerCount];
                        fprintf(stdout, "Reference sum: %.16f\n", RefSum);
                        fprintf(stdout, "Difference: %.16f\n", Sum - RefSum);
                        
                        fprintf(stdout, "\n");
pd.misc_output = profiler::EndCPU(start);
                    }
                }
            }
            
start = profiler::StartCPU();
            FreeBuffer(&ParsedValues);
pd.misc_output = profiler::EndCPU(start);
        }
        else
        {
            fprintf(stderr, "ERROR: Malformed input JSON\n");
        }

start = profiler::StartCPU();
        FreeBuffer(&InputJSON);
        
        Result = 0;
pd.misc_output = profiler::EndCPU(start);

pd.total_time = profiler::EndOS(pd.total_time);
        
        OutputProfilingStats();
    }
    else
    {
        fprintf(stderr, "Usage: %s [haversine_input.json]\n", Args[0]);
        fprintf(stderr, "       %s [haversine_input.json] [answers.f64]\n", Args[0]);
    }
    
    return Result;
}
