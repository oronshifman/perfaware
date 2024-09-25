/* ---------------------------------*/ 
/* Filename: profiler.cpp           */
/* Date:     05.09.2024             */
/* Author:   Oron                   */ 
/* ---------------------------------*/

#include <x86intrin.h>
#include <chrono>

#ifdef WITH_CASEY
typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int32_t b32;

typedef float f32;
typedef double f64;
#endif /* WITH_CASEY */

#include "profiler.h"

namespace profiler
{
	/**
	 * @brief will return the number of OS clock ticks per second
	 */
	u64 GetOSTimerFreq(void)
	{
	    auto tick_period = std::chrono::system_clock::period();

		return tick_period.den / tick_period.num;
	}

	u64 ReadOSTimer(void)
	{
	    return std::chrono::system_clock::now().time_since_epoch().count();
	}

    /**
     * @brief starts os clocks timing
     * @return u64 of the os clock value when this function was called
     */
    u64 StartOS(void)
	{
		return ReadOSTimer();
	}

    /**
     * @brief end cpu clock timing
     * @param start the os clock value of the start of the current timing
     * @return u64 of the os clocks that have taken place between the call to 
     *         StartOS and the call to EndOS
     */
    u64 EndOS(u64 start)
	{
		return ReadOSTimer() - start;
	}

	u64 ReadCPUTimer(void)
	{
	    return __rdtsc();
	}

	/**
	 * @brief will return an estimate for the cpu clock frequency
	 * @param ms_wait time to take while estimating the cpu frequency. the longer the time the
	 *        accurate the estimation
	 */
	u64 GetCPUFreq(u64 ms_wait)
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
	
	/**
     * @brief starts cpu clocks timing
     * @return u64 of the RDTSC value when this function was called
     */ 
	u64 StartCPU(void)
	{
		return ReadCPUTimer();
	}
    
	/**
     * @brief end cpu clock timing
     * @param start the RDTSC value od the start of the current timing
     * @return u64 of the cpu clocks that have taken place between the call to 
     *         StartCPU and the call to EndCPU
     */ 
	u64 EndCPU(u64 start)
	{
		return ReadCPUTimer() - start;
	}
}