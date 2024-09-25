/* ---------------------------------*/ 
/* Filename: haversine_calc.h       */
/* Date:     25.09.2024             */
/* Author:   Oron                   */ 
/* ---------------------------------*/

#ifndef __HAVERSINE_CALC_H__
#define __HAVERSINE_CALC_H__

#include "JSONObject.h"
#include "JSONParser.h"

f64 HaversineCalc(const JSORON::JSONObject& json);

char *ReadEntireFile(std::ifstream& in, const std::string& filename);

/**
 * @brief gets the size of a file from file name
 * @return on success returns the file size if failed return 0
 */
u64 GetFileSize(std::ifstream& in);

u64 GetFileSize(const std::string& filename);

#endif /* HAVERSINE_CALC_H */

