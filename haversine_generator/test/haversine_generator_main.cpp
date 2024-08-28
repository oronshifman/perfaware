/* ------------------------------------------*/ 
/* Filename: haversine_generator_main.cpp    */
/* Date:     17.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include <iostream>
#include <unordered_map>
#include <cmath>

#include "main.h"
#include "haversine_generator.h"
#include "my_int.h"

static exec_option SetExecOption(const std::string user_choice);

int main(int argc, char *argv[])
{
	if (argc < 4)
	{
		std::cerr << "Usage: haversine_generator <option> <seed> <number_of_point>\n";
		std::cerr << "option: uniform, cluster\n";
		return 1;
	}

	exec_option user_choice = SetExecOption(argv[1]);
	
	s64 seed = atoi(argv[2]);
	if (seed < 0)
	{
		std::cerr << "Usage: <seed> must be a positive number\n";
	}
	u64 num_point = atoll(argv[3]);

	if (num_point <= 0)
	{
		std::cout << "Usage: <number_of_point> must be more then 0\n";
		return 1;
	}

	if (num_point > std::pow(2, 32))
	{
		std::cerr << "Usage: <number_of_points> most be less the 2^32\n";
	}

	HaversineJSONGenerator generator;
	if (!generator.GeneratePoints(user_choice, seed, num_point))
	{
		return 1;
	}
	std::cout << "Expected sum: " << generator.expected_sum << "\n";

	return 0;
}

static exec_option SetExecOption(const std::string user_choice)
{
	std::unordered_map<std::string, exec_option> exec_options
	{
		{"uniform", UNIFORM}, {"cluster", CLUSTER}
	};

	return exec_options[user_choice];
}
