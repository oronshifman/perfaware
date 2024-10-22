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

#define _NameConcat(A, B) A##B
#define NameConcat(A, B) _NameConcat(A, B)

#define MAX_ANCHORS 4096
#define UPDATE_COUNTER ({static u8 _local_counter_ = 0;\
                         if (!_local_counter_)\
                            ++Profiler::g_call_id;\
                         _local_counter_ = 1;})

#define Profiler_TimeBlock(block_name) UPDATE_COUNTER; Profiler::ProfilingData NameConcat(profiling_data_, __LINE__)(block_name, Profiler::g_call_id);
#define Profiler_TimeFunction Profiler_TimeBlock(__func__)

class Profiler
{
public:
    class ProfilingData
    {
    public:
        u64 total_tsc;
        u64 children_tsc;
        std::string block_name;
        u16 index;
        u16 index_of_parent;

        ProfilingData() : total_tsc(0), children_tsc(0), block_name("") {}
        ProfilingData(const std::string& block_name_, u16 call_id);
        ~ProfilingData();
    };

private:
    static u64 total_tsc;
    static ProfilingData anchors[MAX_ANCHORS];
    static u16 g_total_parents;

public:
    static u16 g_call_id;

    /**
     * @brief start the timing in wall clock time for the profiling
    */
    static void BeginProfiling();

    /**
     * @brief takes the end timing for the wall clock timer and prints out all the profiling data that
     *        was collected during profiling
     */
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

