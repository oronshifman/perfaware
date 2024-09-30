/* ---------------------------------*/ 
/* Filename: profiler_main.cpp      */
/* Date:     05.09.2024             */
/* Author:   Oron                   */ 
/* ---------------------------------*/

#include <chrono>
#include <iostream>

#include "profiler.h"

void TimingOS(void);
void TimingCPU(void);
void TimingCPU_TimeToWait(u64 ms_wait);
void TestGetCPUFreq(void);

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		TimingCPU_TimeToWait(1000);
	}
	else
	{
		u64 ms_wait = atol(argv[1]);
		TimingCPU_TimeToWait(ms_wait);
	}

	std::cout << "\nTest GetCPUFreq()\n";
	TestGetCPUFreq();

	return 0;
}

void TestGetCPUFreq(void)
{
	std::cout << "1000ms: " << Profiler::GetCPUFreq(1000) << "\n"
			  << " 100ms: " << Profiler::GetCPUFreq(100) << "\n"
			  << "  10ms: " << Profiler::GetCPUFreq(10) << "\n";
}

void TimingOS(void)
{
	u64 os_freq = Profiler::GetOSTimerFreq();
	
	u64 os_start = Profiler::ReadOSTimer();
	u64 os_elapsed = 0;
	u64 os_end = 0;
	while (os_elapsed < os_freq)
	{
		os_end = Profiler::ReadOSTimer();
		os_elapsed = os_end - os_start;
	}

	std::cout << "OS Freq: " << os_freq << "\n"
			  << "  Timer: " << os_start << " -> " << os_end << " = " << os_elapsed << "\n"
			  << "Seconds: " << (f64)os_elapsed / (f64)os_freq << "\n";
}

void TimingCPU(void)
{
	u64 os_freq = Profiler::GetOSTimerFreq();
	
	u64 os_start = Profiler::ReadOSTimer();
	u64 cpu_start = Profiler::ReadCPUTimer();
	u64 os_elapsed = 0;
	u64 os_end = 0;
	while (os_elapsed < os_freq)
	{
		os_end = Profiler::ReadOSTimer();
		os_elapsed = os_end - os_start;
	}

	u64 cpu_end = Profiler::ReadCPUTimer();
	u64 cpu_elapsed = cpu_end - cpu_start;

	std::cout << "  OS Freq: " << os_freq << "\n"
			  << " OS Timer: " << os_start << " -> " << os_end << " = " << os_elapsed << "\n"
			  << "  Seconds: " << (f64)os_elapsed / (f64)os_freq << "\n"
			  << "CPU Timer: " << cpu_start << " -> " << cpu_end << " = " << cpu_elapsed << "\n";
}

/**
 * @brief get CPU freq in a given time interval in milliseconds
 * @param ms_wait the time to sample the cpu clock in milliseconds
 */
void TimingCPU_TimeToWait(u64 ms_wait)
{
	u64 os_freq = Profiler::GetOSTimerFreq();
	
	u64 os_start = Profiler::ReadOSTimer();
	u64 cpu_start = Profiler::ReadCPUTimer();
	u64 os_elapsed = 0;
	u64 os_end = 0;
	u64 os_wait_time = os_freq * ms_wait / 1000; // NOTE(08.09.24): 1000 milliseconds in 1 second
	while (os_elapsed < os_wait_time)
	{
		os_end = Profiler::ReadOSTimer();
		os_elapsed = os_end - os_start;
	}

	u64 cpu_end = Profiler::ReadCPUTimer();
	u64 cpu_elapsed = cpu_end - cpu_start;
	u64 cpu_freq = 0;
	if (os_elapsed)
	{
		cpu_freq = os_freq * cpu_elapsed / os_elapsed;
	}

	std::cout << "  OS Freq: " << os_freq << "\n"
			  << " OS Timer: " << os_start << " -> " << os_end << " = " << os_elapsed << "\n"
			  << "  Seconds: " << (f64)os_elapsed / (f64)os_freq << "\n"
			  << "CPU Timer: " << cpu_start << " -> " << cpu_end << " = " << cpu_elapsed << "\n"
			  << " CPU Freq: " << cpu_freq  << " (guessed)" << "\n";
}