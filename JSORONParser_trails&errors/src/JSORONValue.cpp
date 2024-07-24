/* ------------------------------------------*/ 
/* Filename: JSORONValue.cpp                 */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include <vector>

#include "JSORONValue.h"

template<typename T>
static void CopyVector(std::vector<T>& dest, const std::vector<T>& src);

JSORONValue::JSORONValue(const std::vector<u32>& arr)
{
    type = JSORONType::INT_ARR;
    CopyVector<u32>(int_arr, arr);
}

JSORONValue::JSORONValue(const std::vector<f64>& arr)
{
    type = JSORONType::FLOAT_ARR;
    CopyVector<f64>(float_arr, arr);
}

JSORONValue::JSORONValue(const std::vector<std::string>& arr)
{
    type = JSORONType::STR_ARR;
    CopyVector<std::string>(str_arr, arr);
}

JSORONValue::JSORONValue(const std::vector<JSORONObject>& arr)
{
    type = JSORONType::OBJ_ARR;
    CopyVector<JSORONObject>(obj_arr, arr);
}

template<typename T>
static void CopyVector(std::vector<T>& dest, const std::vector<T>& src)
{
    dest.reserve(src.size());

    for (u64 index = 0; index < src.size(); ++index)
    {
        dest[index] = src[index];
    }
}
