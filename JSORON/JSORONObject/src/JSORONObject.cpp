/* ------------------------------------------*/ 
/* Filename: JSORONObject.cpp                */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include <iomanip>
#include <string>
#include <iostream>

#include "JSORONObject.h"

JSORONObject::JSORONValue::JSORONValue(const JSORONObject &value) : type(JSORONType::JSORON_OBJECT)
{
    json_val = new JSORONObject(value);
}

JSORONObject::JSORONValue::JSORONValue(const JSORONValue &value)
{
    switch (value.type)
    {   
        case JSORONObject::JSORONType::NULL_TYPE:
        {
        } break;

        case JSORONObject::JSORONType::INT:
        {
            type = JSORONType::INT;
            int_val = value.int_val;
        } break;

        case JSORONObject::JSORONType::DOUBLE:
        {
            type = JSORONType::DOUBLE;
            double_val = value.double_val;
        } break;

        case JSORONObject::JSORONType::STR:
        {
            type = JSORONType::STR;
            str_val = value.str_val;
        } break;

        case JSORONObject::JSORONType::JSORON_OBJECT:
        {
            type = JSORONType::JSORON_OBJECT;
            json_val = value.json_val;
        } break;

        case JSORONObject::JSORONType::INT_ARR:
        {
            type = JSORONType::INT_ARR;
            new (&int_arr) std::vector<s32>(value.int_arr);
        } break;

        case JSORONObject::JSORONType::DOUBLE_ARR:
        {   
            type = JSORONType::DOUBLE_ARR;
            new (&double_arr) std::vector<f64>(value.double_arr);
        } break;

        case JSORONObject::JSORONType::STR_ARR:
        {
            type = JSORONType::STR_ARR;
            new (&str_arr) std::vector<std::string>(value.str_arr);
        } break;

        case JSORONObject::JSORONType::OBJ_ARR:
        {
            type = JSORONType::OBJ_ARR;
            new (&obj_arr) std::vector<JSORONObject*>(value.obj_arr);
        } break;

        case JSORONObject::JSORONType::NUM_JSORON_TYPES:
        {
            type = JSORONType::NULL_TYPE;
        } break;
    }
}

JSORONObject::JSORONValue::~JSORONValue()
{
    switch (type)
    {
        case JSORONType::NULL_TYPE:
        case JSORONType::INT:
        case JSORONType::DOUBLE:
        case JSORONType::NUM_JSORON_TYPES:
        {

        } break;

        case JSORONType::STR:
        {
            // str_val.~basic_string();
        } break;
        
        case JSORONType::JSORON_OBJECT:
        {
            delete json_val;
        } break;

        case JSORONType::INT_ARR:
        {
            int_arr.~vector<s32>();
        } break;

        case JSORONType::DOUBLE_ARR:
        {
            double_arr.~vector<f64>();
        } break;

        case JSORONType::STR_ARR:
        {
            str_arr.~vector<std::string>();
        } break;

        case JSORONType::OBJ_ARR:
        {
            for (JSORONObject *obj : obj_arr)
            {
                delete obj;
            }
            obj_arr.~vector<JSORONObject*>();
        } break;
    }
}

JSORONObject::~JSORONObject()
{
    // TODO(24.07.24): impl
}


JSORONObject &JSORONObject::AddObj(const std::string &key)
{
    JSORONObject *new_obj = new JSORONObject();
    return *new_obj;
}   

void JSORONObject::Remove(std::string key)
{
    // TODO(28.07.24): impl
}

void JSORONObject::Print()
{
    std::cout << "{\n";
    RecPrint(1);
    std::cout << "}\n";
}

void JSORONObject::RecPrint(u8 indent)
{
    for (std::string key : insertion_order)
    {
        const JSORONObject::JSORONValue* value = json.at(key);

        std::cout << std::string(indent, '\t') << "\"" + key + "\": ";
        switch (value->type)
        {
            case JSORONObject::JSORONType::NULL_TYPE:
            {
            } break;

            case JSORONObject::JSORONType::INT:
            {
                std::cout << value->int_val << "\n";
            } break;

            case JSORONObject::JSORONType::DOUBLE:
            {
                std::cout << value->double_val << "\n";
            } break;

            case JSORONObject::JSORONType::STR:
            {
                std::cout << std::quoted(value->str_val) << "\n";
            } break;

            case JSORONObject::JSORONType::JSORON_OBJECT:
            {
                std::cout << "{\n";
                value->json_val->RecPrint(indent + 1);
                std::cout << std::string(indent, '\t') << "}\n";
            } break;

            case JSORONObject::JSORONType::INT_ARR:
            {
                std::cout << "[";
                for (u64 index = 0; index < value->int_arr.size() ; ++index)
                {
                    std::cout << value->int_arr[index] << 
                           (index == value->int_arr.size() - 1 ? "]" : ",");
                }
                std::cout << "\n";
            } break;

            case JSORONObject::JSORONType::DOUBLE_ARR:
            {   
                std::cout << "[";
                for (u64 index = 0; index < value->double_arr.size() ; ++index)
                {
                    std::cout << value->double_arr[index] << 
                           (index == value->double_arr.size() - 1 ? "]" : ",");
                }
                std::cout << "\n";
            } break;

            case JSORONObject::JSORONType::STR_ARR:
            {
                std::cout << "[";
                for (u64 index = 0; index < value->str_arr.size() ; ++index)
                {
                    std::cout << value->str_arr[index] << 
                           (index == value->str_arr.size() - 1 ? "]" : ",");
                }
                std::cout << "\n";
            } break;

            case JSORONObject::JSORONType::OBJ_ARR:
            {
                std::cout << "[\n";
                ++indent;
                for (u64 index = 0; index < value->obj_arr.size() ; ++index)
                {
                    std::cout << std::string(indent, '\t') << "{\n";
                    value->obj_arr[index]->RecPrint(indent + 1);
                    std::cout << std::string(indent, '\t') << "}";
                    std::cout << (index != value->obj_arr.size() - 1 ? ",\n" : "\n");
                }
                std::cout << std::string(--indent, '\t') << "]\n";
            } break;

            case JSORONObject::JSORONType::NUM_JSORON_TYPES:
            {
            } break;
        }
    }
}
