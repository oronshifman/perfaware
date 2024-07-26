/* ------------------------------------------*/ 
/* Filename: JSORONObject.cpp                */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include <string>

#include "JSORONObject.h"

JSORONObject::JSORONValue::JSORONValue(const JSORONObject &value)
{
    type = JSORONType::JSORON_OBJECT;
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
            int_val = value.int_val;
        } break;

        case JSORONObject::JSORONType::DOUBLE:
        {
            double_val = value.double_val;
        } break;

        case JSORONObject::JSORONType::STR:
        {
            str_val = value.str_val;
        } break;

        case JSORONObject::JSORONType::JSORON_OBJECT:
        {
            json_val = value.json_val;
        } break;

        case JSORONObject::JSORONType::INT_ARR:
        {
            new (&int_arr) std::vector<s32>(value.int_arr);
        } break;

        case JSORONObject::JSORONType::DOUBLE_ARR:
        {   
            new (&double_arr) std::vector<f64>(value.double_arr);
        } break;

        case JSORONObject::JSORONType::STR_ARR:
        {
            new (&str_arr) std::vector<std::string>(value.str_arr);
        } break;

        case JSORONObject::JSORONType::OBJ_ARR:
        {
            new (&obj_arr) std::vector<JSORONObject*>(value.obj_arr);
        } break;

        case JSORONObject::JSORONType::NUM_JSORON_TYPES:
        {
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
            str_val.~basic_string();
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
                obj->~JSORONObject();
            }
            obj_arr.~vector<JSORONObject*>();
        } break;
    }
}

JSORONObject::JSORONObject(const JSORONObject& value)
{
    // TODO(25.07.24): this looks to be kind of circular... how do i create a 
    // JSORONValue that hols a JSORONObject?
}

JSORONObject::~JSORONObject()
{
    // TODO(24.07.24): impl
}

// void JSORONObject::Put(const std::string& key, const s32 value)
// {
//     JSORONValue new_value(value);
//     auto res = json.insert({key, new_value});
//     if (res.second)
//     {
//         insertion_order.push_back(key);
//     }
// }

// void JSORONObject::Put(const std::string& key, const f64 value)
// {
// }

// void JSORONObject::Put(const std::string& key, const std::string value)
// {
// }

// void JSORONObject::Put(const std::string& key, const JSORONObject& value)
// {
// }

// void JSORONObject::Put(const std::string& key, const std::vector<s32>& value)
// {
// }

// void JSORONObject::Put(const std::string& key, const std::vector<f64>& value)
// {

// }

// void JSORONObject::Put(const std::string& key, const std::vector<std::string>& value)
// {

// }

// void JSORONObject::Put(const std::string& key, const std::vector<JSORONObject>& value)
// {

// }

JSORONObject& JSORONObject::AddObj(const std::string &key)
{
    JSORONObject *new_obj = new JSORONObject();
    return *new_obj;
}   

void JSORONObject::Remove(std::string key)
{

}

std::ostream &operator<<(std::ostream &out, const JSORONObject obj)
{
    for (std::string key : obj.insertion_order)
    {
        const JSORONObject::JSORONValue& value = obj.json.at(key);

        out << "\"" + key + "\": ";
        switch (value.type)
        {
            case JSORONObject::JSORONType::NULL_TYPE:
            {
            } break;

            case JSORONObject::JSORONType::INT:
            {
                out << std::to_string(value.int_val) << std::string("\n");
            } break;

            case JSORONObject::JSORONType::DOUBLE:
            {
                out << std::to_string(value.double_val) << std::string("\n");
            } break;

            case JSORONObject::JSORONType::STR:
            {
                out << value.str_val << std::string("\n");
            } break;

            case JSORONObject::JSORONType::JSORON_OBJECT:
            {
                out << *(value.json_val) << std::string("\n");
            } break;

            case JSORONObject::JSORONType::INT_ARR:
            {
                out << std::string("[");
                for (u64 index = 0; index < value.int_arr.size() ; ++index)
                {
                    out << std::to_string(value.int_arr[index]) << 
                           (index == value.int_arr.size() - 1 ? std::string(",") : std::string("]"));
                }
            } break;

            case JSORONObject::JSORONType::DOUBLE_ARR:
            {   
                out << std::string("[");
                for (u64 index = 0; index < value.double_arr.size() ; ++index)
                {
                    out << std::to_string(value.double_arr[index]) << 
                           (index == value.double_arr.size() - 1 ? std::string(",") : std::string("]"));
                }
            } break;

            case JSORONObject::JSORONType::STR_ARR:
            {
                out << std::string("[");
                for (u64 index = 0; index < value.str_arr.size() ; ++index)
                {
                    out << value.str_arr[index] << 
                           (index == value.str_arr.size() - 1 ? std::string(",") : std::string("]"));
                }
            } break;

            case JSORONObject::JSORONType::OBJ_ARR:
            {
                out << std::string("[");
                for (u64 index = 0; index < value.double_arr.size() ; ++index)
                {
                    out << *(value.obj_arr[index]) << 
                           (index == value.double_arr.size() - 1 ? std::string(",") : std::string("]"));
                }
            } break;

            case JSORONObject::JSORONType::NUM_JSORON_TYPES:
            {
            } break;
        }
    }

    return out;
}
