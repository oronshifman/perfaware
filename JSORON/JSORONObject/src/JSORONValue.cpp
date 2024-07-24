/* ------------------------------------------*/ 
/* Filename: JSORONValue.cpp                 */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include <vector>

#include "my_int.h"
#include "JSORONValue.h"

JSORONValue::~JSORONValue()
{
    switch (type)
    {
        case JSORONType::INT:
        case JSORONType::FLOAT:
        case JSORONType::STR:
        {

        } break;
        
        case JSORONType::JSORON_OBJECT:
        {
            json_val.~JSORONObject();
        } break;

        case JSORONType::INT_ARR:
        {
            int_arr.~vector<s32>();
        } break;

        case JSORONType::FLOAT_ARR:
        {
            float_arr.~vector<f64>();
        } break;

        case JSORONType::STR_ARR:
        {
            str_arr.~vector<std::string>();
        } break;

        case JSORONType::OBJ_ARR:
        {
            for (JSORONObject obj : obj_arr)
            {
                obj.~JSORONObject();
            }
            obj_arr.~vector<JSORONObject>();
        } break;
    }
}