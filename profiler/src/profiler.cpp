/* ---------------------------------*/ 
/* Filename: profiler.cpp           */
/* Date:     05.09.2024             */
/* Author:   Oron                   */ 
/* ---------------------------------*/

#include <x86intrin.h>
#include <chrono>
#include <assert.h>

#include "profiler.h"

u64 Profiler::total_tsc = 0;
u16 Profiler::g_call_id = 0;
u16 Profiler::g_total_parents = 0;
Profiler::ProfilingData Profiler::anchors[MAX_ANCHORS] = {};

Profiler::ProfilingData::ProfilingData(const std::string& block_name_, u16 call_id)
{
	assert(g_call_id < MAX_ANCHORS);

	index_of_parent = g_total_parents;

	index = call_id;
	block_name = block_name_; 

	g_total_parents = index;
	total_tsc = ReadCPUTimer();
}

Profiler::ProfilingData::~ProfilingData()
{
	u64 elapsed_tsc = ReadCPUTimer() - total_tsc;
	g_total_parents = index_of_parent;

	ProfilingData *anchor = anchors + index;
	ProfilingData *parent = anchors + index_of_parent;

	anchor->total_tsc += elapsed_tsc;
	parent->children_tsc += elapsed_tsc;

	anchor->block_name = block_name;
	anchor->index = index;

	anchor->index_of_parent = index_of_parent;
}
    
void Profiler::BeginProfiling()
{
	total_tsc = ReadCPUTimer();
}

void Profiler::EndProfilingAndPrint()
{
	total_tsc = ReadCPUTimer() - total_tsc;
	u64 cpu_freq = GetCPUFreq(100);

	fprintf(stdout, "\n\n");
	fprintf(stdout, "Total time: %.4fms (CPU freq: %lu)\n\n", 1000.0 * (f64)total_tsc / (f64)cpu_freq, cpu_freq);
	fprintf(stdout, "Total TSC: %lu\n", total_tsc);

	for (u64 anchor_index = 0; anchor_index < MAX_ANCHORS; ++anchor_index)
	{
		ProfilingData *curr = anchors + anchor_index;
		if (curr->total_tsc)
		{
			u64 exclusive_tsc = curr->total_tsc - curr->children_tsc;
			f64 exclusive_percent = 100.0 * ((f64)exclusive_tsc / (f64)total_tsc);
			fprintf(stdout, "    %s: %lu (%.2f%%)", curr->block_name.c_str(), exclusive_tsc, exclusive_percent);
			if (curr->children_tsc)
			{
				f64 total_percent = 100.0 * ((f64)curr->total_tsc / (f64)total_tsc);
				fprintf(stdout, ", w/children %lu (%.2f%%)", curr->total_tsc, total_percent);
			}
			fprintf(stdout, "\n");
		}
	}
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