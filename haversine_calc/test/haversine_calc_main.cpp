/* ------------------------------------------*/ 
/* Filename: haversine_calc_main.cpp         */
/* Date:     27.08.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include <iostream>
#include <iomanip>

#include "haversine_calc.h"
#include "JSONObject.h"
#include "JSONParser.h"

using namespace JSORON;

int main(int argc, char *argv[])
{
	std::cout << "\n";

	if (argc < 2 || argc > 3)
	{
		std::cerr << "Usage: haversine_calc <pairs.json> <answers.bin>\n";
		std::cerr << "<answers.bin> is optional\n";
		return 1;
	}

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

	char *json_str = nullptr;
	if ((json_str = ReadEntireFile(json_file, argv[1])) == nullptr)
	{
		return 1;
	}

	JSONParser parser;
	JSONObject json_obj = parser.Parse(json_str);
	if (json_obj == JSONParser::bad_obj)
	{
		std::cerr << "ERROR - Failed to parse: " << argv[1] << "\n";
	}

	f64 sum = HaversineCalc(json_obj);

	std::cout << std::fixed << std::setprecision(16);
	std::cout << "Sum: " << sum << "\n";

	// TODO(29.08.24): debug the reference validation. At the moment reading of the expected sum
	// from the reference file is not correct. not sure if the problem is with writing of the 
	// data in the generator or is it the reading...
	if (argc == 3)
	{
		JSONArray pairs = json_obj["pairs"];
		u64 pairs_count = pairs.Size();

		f32 *answers = nullptr;
		if ((answers = (f32 *)ReadEntireFile(answers_file, argv[2])) == nullptr)
		{
			return 1;
		}
		u64 answers_count = (GetFileSize(answers_file) - sizeof(f64)) / sizeof(f32);

		std::cout << "\nValidation:\n";

		if (pairs_count != answers_count)
		{
			std::cerr << "ERROR - Number of pairs don't match number of answers\n";
		}

		f64 ref_sum = (f64)*(answers + answers_count);

		std::cout << "Reference sum: " << ref_sum << "\n";
		std::cout << "Difference: " << sum - ref_sum << "\n";
	
		free(answers);
	}

	free(json_str);

	return 0;
}




























