/* ------------------------------------------*/ 
/* Filename: JSORONValue.h                   */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/*         FOR TESTING JSORONValue ONLY      */
/* ------------------------------------------*/

#ifndef __JSORON_VALUE_H__
#define __JSORON_VALUE_H__

#include <vector>
#include <string>

#include "my_int.h"

class JSORONObject;

enum class JSORONType
{
    NULL_TYPE,

    INT,
    FLOAT,
    STR,
    JSORON_OBJECT,

    INT_ARR,
    FLOAT_ARR,
    STR_ARR,
    OBJ_ARR,

    NUM_JSORON_TYPES
};

class JSORONValue 
{
public:
    JSORONType type;

    union
    {
        s32 int_val;
        f64 float_val;
        std::string str_val;
        JSORONObject *json_val;

        std::vector<s32> int_arr;
        std::vector<f64> float_arr;
        std::vector<std::string> str_arr;
        std::vector<JSORONObject*> obj_arr;
    };

    JSORONValue() : type(JSORONType::NULL_TYPE) {}
    ~JSORONValue();

    JSORONValue(const s32 value) : type(JSORONType::INT), int_val(value) {}
    JSORONValue(const f64 value) : type(JSORONType::FLOAT), float_val(value) {}
    JSORONValue(const std::string value) : type(JSORONType::STR), str_val(value) {}
    JSORONValue(const JSORONObject &value);

    JSORONValue(const std::vector<s32>& arr) : type(JSORONType::INT_ARR), int_arr(arr) {}
    JSORONValue(const std::vector<f64>& arr) : type(JSORONType::FLOAT_ARR), float_arr(arr) {}
    JSORONValue(const std::vector<std::string>& arr) : type(JSORONType::STR_ARR), str_arr(arr) {}
    JSORONValue(const std::vector<JSORONObject*>& arr) : type(JSORONType::OBJ_ARR), obj_arr(arr) {}
};

#endif /* JSORON_VALUE_H */

