/* ------------------------------------------*/ 
/* Filename: JSORONValue.h                   */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#ifndef __JSORON_VALUE_H__
#define __JSORON_VALUE_H__

#include <vector>
#include <string>

#include "my_int.h"
#include "JSORONObject.h"

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
        u32 int_val;
        f64 float_val;
        std::string str_val;
        JSORONObject json_val;

        std::vector<u32> int_arr;
        std::vector<f64> float_arr;
        std::vector<std::string> str_arr;
        std::vector<JSORONObject> obj_arr;
    };

    JSORONValue() : type(JSORONType::NULL_TYPE) {}

    JSORONValue(const u32 value) : type(JSORONType::INT), int_val(value) {}
    JSORONValue(const f64 value) : type(JSORONType::FLOAT), float_val(value) {}
    JSORONValue(const std::string value) : type(JSORONType::STR), str_val(value) {}
    JSORONValue(const JSORONObject value) : type(JSORONType::JSORON_OBJECT), json_val(value) {}

    JSORONValue(const std::vector<u32>& arr);
    JSORONValue(const std::vector<f64>& arr);
    JSORONValue(const std::vector<std::string>& arr);
    JSORONValue(const std::vector<JSORONObject>& arr);
};

#endif /* JSORON_VALUE_H */

