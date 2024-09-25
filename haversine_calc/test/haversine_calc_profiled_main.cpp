/* -------------------------------------------*/ 
/* Filename: haversine_calc_profiled_main.cpp */
/* Date:     27.08.2024                       */
/* Author:   Oron                             */ 
/* -------------------------------------------*/

#include <iostream>
#include <iomanip>

#include "haversine_calc.h"
#include "JSONObject.h"
#include "JSONParser.h"
#include "profiler.h"

using namespace JSORON;

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

void OutputProfilingStats();

int main(int argc, char *argv[])
{
	pd.cpu_freq = profiler::GetCPUFreq(100);
	pd.os_freq = profiler::GetOSTimerFreq();
	u64 start = 0;

	/********PROFILING**********/
	pd.total_time = profiler::StartOS();
	/********PROFILING**********/

	/********PROFILING**********/
	start = profiler::StartCPU();
	/********PROFILING**********/
	std::cout << "\n";
	/********PROFILING**********/
	pd.misc_output = profiler::EndCPU(start);
	/********PROFILING**********/

	/********PROFILING**********/
	start = profiler::StartCPU();	
	/********PROFILING**********/
	if (argc < 2 || argc > 3)
	{
		std::cerr << "Usage: haversine_calc <pairs.json> <answers.bin>\n";
		std::cerr << "<answers.bin> is optional\n";
		return 1;
	}
	/********PROFILING**********/
	pd.startup = profiler::EndCPU(start);	
	/********PROFILING**********/

	/********PROFILING**********/
	start = profiler::StartCPU();
	/********PROFILING**********/
	std::ifstream json_file;
	json_file.open(argv[1]);
	if (!json_file.good())
	{
		std::cerr << argv[1] << " not found\n";
	}

	std::ifstream answers_file;
	if (argc == 3)
	{
		answers_file.open(argv[2], std::ios::binary);
		if (!answers_file.good())
		{
			std::cerr << argv[2] << " not found\n";
		}
	}
	/********PROFILING**********/
	pd.misc_setup = profiler::EndCPU(start);
	/********PROFILING**********/

	/********PROFILING**********/
	start = profiler::StartCPU();
	/********PROFILING**********/
	char *json_str = nullptr;
	if ((json_str = ReadEntireFile(json_file, argv[1])) == nullptr)
	{
		return 1;
	}
	json_str[GetFileSize(json_file)] = '\0';
	/********PROFILING**********/
	pd.read = profiler::EndCPU(start);
	/********PROFILING**********/
	
	/********PROFILING**********/
	start = profiler::StartCPU();
	/********PROFILING**********/
	json_file.close();
	/********PROFILING**********/
	pd.misc_setup += profiler::EndCPU(start);
	/********PROFILING**********/

	/********PROFILING**********/
	start = profiler::StartCPU();
	/********PROFILING**********/
	JSONParser parser;
	JSONObject json_obj = parser.Parse(json_str);
	if (json_obj == JSONParser::bad_obj)
	{
		std::cerr << "ERROR - Failed to parse: " << argv[1] << "\n";
	}
	/********PROFILING**********/
	pd.parse = profiler::EndCPU(start);
	/********PROFILING**********/

	/********PROFILING**********/
	start = profiler::StartCPU();
	/********PROFILING**********/
	f64 sum = HaversineCalc(json_obj);
	/********PROFILING**********/
	pd.sum = profiler::EndCPU(start);
	/********PROFILING**********/

	
	/********PROFILING**********/
	start = profiler::StartCPU();
	/********PROFILING**********/
	std::cout << std::fixed << std::setprecision(16);
	std::cout << "Sum: " << sum << "\n";
	/********PROFILING**********/
	pd.misc_output = profiler::EndCPU(start);
	/********PROFILING**********/
	if (argc == 3)
	{
		/********PROFILING**********/
		start = profiler::StartCPU();
		/********PROFILING**********/
		JSONArray pairs = json_obj["pairs"];
		u64 pairs_count = pairs.Size();
		/********PROFILING**********/
		pd.misc_setup += profiler::EndCPU(start);
		/********PROFILING**********/

		/********PROFILING**********/
		start = profiler::StartCPU();
		/********PROFILING**********/
		f32 *answers = nullptr;
		if ((answers = (f32 *)ReadEntireFile(answers_file, argv[2])) == nullptr)
		{
			return 1;
		}
		/********PROFILING**********/
		pd.read += profiler::EndCPU(start);
		/********PROFILING**********/

		/********PROFILING**********/
		start = profiler::StartCPU();
		/********PROFILING**********/
		u64 answers_count = (GetFileSize(answers_file) - sizeof(f64)) / sizeof(f32);
		/********PROFILING**********/
		pd.read += profiler::EndCPU(start);
		/********PROFILING**********/

		/********PROFILING**********/
		start = profiler::StartCPU();
		/********PROFILING**********/
		answers_file.close();
		/********PROFILING**********/
		pd.misc_setup += profiler::EndCPU(start);
		/********PROFILING**********/

		std::cout << "\nValidation:\n";

		if (pairs_count != answers_count)
		{
			std::cerr << "ERROR - Number of pairs don't match number of answers\n";
		}

		f64 ref_sum = *((f64*)(answers + answers_count));

		std::cout << "Reference sum: " << ref_sum << "\n";
		std::cout << "Difference: " << sum - ref_sum << "\n";
	
		free(answers);
	}

	/********PROFILING**********/
	start = profiler::StartCPU();
	/********PROFILING**********/
	free(json_str);
	/********PROFILING**********/
	pd.misc_setup += profiler::EndCPU(start);
	/********PROFILING**********/

	/********PROFILING**********/
	pd.total_time = profiler::EndOS(pd.total_time);
	/********PROFILING**********/

	OutputProfilingStats();

	return 0;
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


























