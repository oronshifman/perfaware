/* --------------------------------*/ 
/* Filename: profiler.h            */
/* Date:     05.09.2024            */
/* Author:   Oron                  */ 
/* --------------------------------*/

#ifndef __PROFILER_H__
#define __PROFILER_H__

#include <my_int.h>

namespace profiler
{
    /**
     * @brief will return a reading of the OS timer since epoch
     */
    u64 ReadOSTimer(void);

    /**
     * @brief will return the number of OS clock ticks per second
     */
    u64 GetOSTimerFreq(void);

    /**
     * @brief starts os clocks timing
     * @return u64 of the os clock value when this function was called
     */
    u64 StartOS(void);

    /**
     * @brief end cpu clock timing
     * @param start the os clock value of the start of the current timing
     * @return u64 of the os clocks that have taken place between the call to 
     *         StartOS and the call to EndOS
     */
    u64 EndOS(u64 start);

    u64 ReadCPUTimer(void);

    /**
     * @brief will return an estimate for the cpu clock frequency
     * @param ms_wait time to take while estimating the cpu frequency. the longer the time the
     *        accurate the estimation
     */
    u64 GetCPUFreq(u64 ms_wait);

    /**
     * @brief starts cpu clocks timing
     * @return u64 of the RDTSC value when this function was called
     */
    u64 StartCPU(void);

    /**
     * @brief end cpu clock timing
     * @param start the RDTSC value od the start of the current timing
     * @return u64 of the cpu clocks that have taken place between the call to 
     *         StartCPU and the call to EndCPU
     */
    u64 EndCPU(u64 start);
}
#endif /* PROFILER_H */

