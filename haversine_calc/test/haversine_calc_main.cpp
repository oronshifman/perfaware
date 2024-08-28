/* ------------------------------------------*/ 
/* Filename: haversine_calc_main.cpp         */
/* Date:     27.08.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include <iostream>

#include "haversine_calc.h"
#include "JSONObject.h"
#include "JSONParser.h"

using namespace JSORON;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: haversine_calc <pairs.json> <answers.bin>\n";
		std::cerr << "<answers.bin> is optional\n";
		return 1;
	}

	std::ifstream json;
	json.open(argv[1]);
	if (!json.good())
	{
		std::cerr << argv[1] << " not found\n";
	}

	std::ifstream answers;
	if (argc == 3)
	{
		answers.open(argv[2]);
		if (!answers.good())
		{
			std::cerr << argv[2] << " not found\n";
		}
	}

	// TODO(27.08.24): continue impl
	std::string *json_str;
	if ((json_str = ReadEntireFile(json, argv[1])) == nullptr)
	{
		return 1;
	}

	JSONParser parser;
	JSONObject json = parser.Parse(*json_str);
	if (json == JSONParser::bad_obj)
	{
		std::cerr << "Failed to parse: " << argv[1] << "\n";
	}

	return 0;
}




























