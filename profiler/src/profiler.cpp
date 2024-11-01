/* ---------------------------------*/ 
/* Filename: profiler.cpp           */
/* Date:     05.09.2024             */
/* Author:   Oron                   */ 
/* ---------------------------------*/

#include <x86intrin.h>
#include <chrono>
#include <assert.h>

#include "profiler.h"

u64 Profiler::g_total_tsc = 0;

#if PROFILING
u16 Profiler::g_call_id = 0;
u16 Profiler::g_total_parents = 0;
profile_anchor_t Profiler::anchors[MAX_ANCHORS] = {0};

Profiler::ProfilingData::ProfilingData(const std::string& block_name_, u64 byte_count, u16 call_id)
{
	assert(g_call_id < MAX_ANCHORS);

	index_of_parent = g_total_parents;

	index = call_id;
	block_name = block_name_; 

	profile_anchor_t *anchor = anchors + index;
	old_tsc_inclusive = anchor->tsc_inclusive;
	anchor->bytes_processed += byte_count;

	g_total_parents = index;
	start_tsc = ReadCPUTimer();
}

Profiler::ProfilingData::~ProfilingData()
{
	u64 elapsed_tsc = ReadCPUTimer() - start_tsc;
	g_total_parents = index_of_parent;

	profile_anchor_t *anchor = anchors + index;
	profile_anchor_t *parent = anchors + index_of_parent;

	parent->tsc_exclusive -= elapsed_tsc;
	anchor->tsc_exclusive += elapsed_tsc;
	anchor->tsc_inclusive = old_tsc_inclusive + elapsed_tsc;
	++(anchor->hit_count);

	anchor->block_name = block_name;
}
#endif /* PROFILING */    

void Profiler::BeginProfiling()
{
	g_total_tsc = ReadCPUTimer();
}

void Profiler::EndProfilingAndPrint()
{
	g_total_tsc = ReadCPUTimer() - g_total_tsc;
	u64 cpu_freq = GetCPUFreq(100);

	printf("\n\n");
	printf("Total time: %.4fms (CPU freq: %lu)\n\n", 1000.0 * (f64)g_total_tsc / (f64)cpu_freq, cpu_freq);
	printf("Total TSC: %lu\n", g_total_tsc);

#if PROFILING
	for (u64 anchor_index = 1; anchor_index < MAX_ANCHORS; ++anchor_index)
	{
		profile_anchor_t *curr = anchors + anchor_index;
		if (curr->tsc_exclusive)
		{
			f64 exclusive_percent = 100.0 * ((f64)curr->tsc_exclusive / (f64)g_total_tsc);
			printf("	%s[%lu]: %lu (%.2f%%)", curr->block_name.c_str(), curr->hit_count, curr->tsc_exclusive, exclusive_percent);
			if (curr->tsc_exclusive != curr->tsc_inclusive)
			{
				f64 percent_with_children = 100.0 * ((f64)curr->tsc_inclusive / (f64)g_total_tsc);
				printf(", w/children %lu (%.2f%%)", curr->tsc_inclusive, percent_with_children);
			}

			if (curr->bytes_processed)
			{
				f64 megabyte = 1024.0f*1024.0f;
				f64 gigabyte = megabyte*1024.0f;

				f64 elapsed_seconds = (f64)curr->tsc_inclusive / (f64)cpu_freq;
				f64 byte_per_second = (f64)curr->bytes_processed / (f64)elapsed_seconds;
				f64 megabytes_processed = (f64)curr->bytes_processed / (f64)megabyte;
				f64 gigabytes_per_second = (f64)byte_per_second / (f64)gigabyte;

				printf(" %.3fmb at %.5fgb/s", megabytes_processed, gigabytes_per_second);
			}

			printf("\n");
		}
	}
#endif /* PROFILING */
}

/**
 * @brief will return the number of OS clock ticks per second
 */
u64 Profiler::GetOSTimerFreq(void)
{
    auto tick_period = std::chrono::system_clock::period();

	return tick_period.den / tick_period.num;
}

u64 Profiler::ReadOSTimer(void)
{
    return std::chrono::system_clock::now().time_since_epoch().count();
}

u64 Profiler::ReadCPUTimer(void)
{
    return __rdtsc();
}

/**
 * @brief will return an estimate for the cpu clock frequency
 * @param ms_wait time to take while estimating the cpu frequency. the longer the time the
 *        accurate the estimation
 */
u64 Profiler::GetCPUFreq(u64 ms_wait)
{
	u64 os_freq = GetOSTimerFreq();

	u64 os_start = ReadOSTimer();
	u64 cpu_start = ReadCPUTimer();
	u64 os_elapsed = 0;
	u64 os_end = 0;
	u64 os_wait_time = os_freq * ms_wait / 1000; // NOTE(08.09.24): 1000 milliseconds in 1 second
	while (os_elapsed < os_wait_time)
	{
		os_end = ReadOSTimer();
		os_elapsed = os_end - os_start;
	}

	u64 cpu_end = ReadCPUTimer();
	u64 cpu_elapsed = cpu_end - cpu_start;
	u64 cpu_freq = 0;
	if (os_elapsed)
	{
		cpu_freq = os_freq * cpu_elapsed / os_elapsed;
	}

    return cpu_freq;
}