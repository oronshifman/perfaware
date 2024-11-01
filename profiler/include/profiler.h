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

#ifndef PROFILING
#define PROFILING 0
#endif /* PROFILING */

#if PROFILING
#define _NameConcat(A, B) A##B
#define NameConcat(A, B) _NameConcat(A, B)

#define MAX_ANCHORS 4096
#define UPDATE_COUNTER(local_id) \
            static u8 is_first_pass = 0;\
            if (!is_first_pass)\
            {\
                ++Profiler::g_call_id;\
                local_id = Profiler::g_call_id;\
            }\
            is_first_pass = 1;

#define Profiler_TimeBandwidth(block_name, byte_count) \
            static u16 local_id; \
            UPDATE_COUNTER(local_id); \
            Profiler::ProfilingData NameConcat(profiling_data_, __LINE__)(block_name, byte_count, local_id);
#define Profiler_TimeBlock(block_name) Profiler_TimeBandwidth(block_name, 0)
#define Profiler_TimeFunction Profiler_TimeBandwidth(__func__, 0)

typedef struct profile_anchor
{
    u64 tsc_exclusive;
    u64 tsc_inclusive;
    std::string block_name;
    u64 bytes_processed;
    u64 hit_count;
} profile_anchor_t;
#else

#define Profiler_TimeBlock(...)
#define Profiler_TimeBandwidth(...)
#define Profiler_TimeFunction

#endif /* PROFILING */

class Profiler
{
public:
#if PROFILING
    class ProfilingData
    {
    public:
        u64 start_tsc;
        std::string block_name;
        u64 old_tsc_inclusive;
        u16 index;
        u16 index_of_parent;

        ProfilingData() : start_tsc(0), block_name("") {}
        ProfilingData(const std::string& block_name_, u64 byte_count, u16 call_id);
        ~ProfilingData();
    };
#endif /* PROFILING */

private:
    static u64 g_total_tsc;
#if PROFILING
    static profile_anchor_t anchors[MAX_ANCHORS];
    static u16 g_total_parents;
#endif /* PROFILING */

public:
    static u16 g_call_id;

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

    /**
     * @brief start the timing in wall clock time for the profiling
    */
    static void BeginProfiling();

    /**
     * @brief takes the end timing for the wall clock timer and prints out all the profiling data that
     *        was collected during profiling
     */
    static void EndProfilingAndPrint();
};

#endif /* PROFILER_H */

