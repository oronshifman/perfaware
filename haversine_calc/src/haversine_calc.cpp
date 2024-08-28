/* ------------------------------------------*/ 
/* Filename: haversine_calc.cpp              */
/* Date:     27.08.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include <iostream>

#include "listing_0065_haversine_formula.h"
#include "haversine_calc.h"
#include "JSONObject.h"

using namespace JSORON;

f64 haversine_calc(const JSONObject& json)
{
    // TODO(28.08.24): impl
}

std::string *ReadEntireFile(std::ifstream& in, const std::string& filename)
{
	u64 file_size = 0;
	in.seekg(0, std::ios_base::end);
	file_size = in.tellg();
	in.seekg(0);

	std::string *json_str = new std::string(file_size, ' ');
    
    in.read(&(*json_str)[0], file_size);
	if (in.fail())
    {
        std::cerr << "Failed to read file: " << filename << "\n"; 
	    return nullptr;
    }

    return json_str;
}