/* --------------------------------*/ 
/* Filename: profiler.h            */
/* Date:     05.09.2024            */
/* Author:   Oron                  */ 
/* --------------------------------*/

#ifndef __PROFILER_H__
#define __PROFILER_H__

#include <string>
#include <vector>

#include "my_int.h"

#define MAX_ANCHORS 20
#define TimeFunction Profiler::ProfilingData profiling_data(__func__)
#define TimeBlock(block_name) Profiler::ProfilingData profiling_data(block_name)

class Profiler
{
public:
    class ProfilingData
    {
    public:
        u64 tsc_clocks;
        std::string block_name;
        s32 index;

        ProfilingData() : tsc_clocks(0), block_name(""), index(MAX_ANCHORS + 1){}
        ProfilingData(const std::string& block_name);
        ~ProfilingData();
    };

private:
    static u64 total_time;
    static ProfilingData anchors[MAX_ANCHORS];
    static s8 anchors_index;

public:
    static void BeginProfiling();

    static void EndProfilingAndPrint();

    /**
     * @brief will return a reading of the OS timer since epoch
     */
    static u64 ReadOSTimer(void);

    /**
     * @brief will return the number of OS clock ticks per second
     */
    static u64 GetOSTimerFreq(void);

    static u64 ReadCPUTimer(void);

    /**
     * @brief will return an estimate for the cpu clock frequency
     * @param ms_wait time to take while estimating the cpu frequency. the longer the time the
     *        accurate the estimation
     */
    static u64 GetCPUFreq(u64 ms_wait);
};

#endif /* PROFILER_H */

