/* ------------------------------------------*/ 
/* Filename: JSONObject.cpp                  */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include <ostream>
#include <string>
#include <iostream>

#include "JSONObject.h"

JSONObject::JSONValue JSONObject::bad_value(JSONObject::JSONType::BAD_TYPE);

JSONObject::JSONValue::JSONValue(const JSONValue &value)
{
    AssignValueByType(value);
}

JSONObject::JSONValue::JSONValue(const JSONObject &value) : type(JSONType::JSON_OBJECT)
{
    json_val = new JSONObject(value);
}

JSONObject::JSONValue::JSONValue(const std::vector<JSONObject*>& arr) : type(JSONType::OBJ_ARR)
{
    new (&obj_arr) std::vector<JSONObject*>();
    obj_arr.reserve(arr.size());

    for (JSONObject* obj : arr)
    {
        obj_arr.push_back(new JSONObject(*obj));
    }
}

JSONObject::JSONValue& JSONObject::JSONValue::operator=(const JSONValue& other)
{
    if (this == &other)
    {
        return *this;
    }
    
    AssignValueByType(other);
    return *this;
}

JSONObject::JSONValue::operator int() const
{
    if (type == JSONObject::JSONType::INT)
    {
        return int_val;
    }
    else
    {
        throw std::bad_cast();
    }
}

JSONObject::JSONValue::operator double() const
{
    if (type == JSONObject::JSONType::DOUBLE)
    {
        return double_val;
    } else
    {
        throw std::bad_cast();
    }
}

JSONObject::JSONValue::operator std::string() const
{
    if (type == JSONObject::JSONType::STR)
    {
        return str_val;
    }
    else
    {
        throw std::bad_cast();
    }
}

JSONObject::JSONValue::operator JSONObject*() const
{
    if (type == JSONObject::JSONType::JSON_OBJECT)
    {
        return json_val;
    }
    else
    {
        throw std::bad_cast();
    }
}

JSONObject::JSONValue::operator std::vector<int>() const
{
    if (type == JSONObject::JSONType::INT_ARR)
    {
        return int_arr;
    }
    else
    {
        throw std::bad_cast();
    }
}

JSONObject::JSONValue::operator std::vector<double>() const
{
    if (type == JSONObject::JSONType::DOUBLE_ARR)
    {
        return double_arr;
    }
    else
    {
        throw std::bad_cast();
    }
}

JSONObject::JSONValue::operator std::vector<std::string>() const
{
    if (type == JSONObject::JSONType::STR_ARR)
    {
        return str_arr;
    }
    else
    {
        throw std::bad_cast();
    }
}

JSONObject::JSONValue::operator std::vector<JSONObject*>() const
{
    if (type == JSONObject::JSONType::OBJ_ARR)
    {
        return obj_arr;
    }
    else
    {
        throw std::bad_cast();
    }
}

void JSONObject::JSONValue::PrintValueByType(u8 indent, std::ostream& out) const
{
        switch (type)
        {
            case JSONObject::JSONType::BAD_TYPE:
            case JSONObject::JSONType::NULL_TYPE:
            {
            } break;

            case JSONObject::JSONType::INT:
            {
                out << int_val << "\n";
            } break;

            case JSONObject::JSONType::DOUBLE:
            {
                out << double_val << "\n";
            } break;

            case JSONObject::JSONType::STR:
            {
                out << "\"" << str_val << "\"" << "\n";
            } break;

            case JSONObject::JSONType::JSON_OBJECT:
            {
                out << "{\n";
                json_val->RecPrint(indent + 1, out);
                out << std::string(indent, '\t') << "}\n";
            } break;

            case JSONObject::JSONType::INT_ARR:
            {
                out << "[";
                for (u64 index = 0; index < int_arr.size() ; ++index)
                {
                    out << int_arr[index] << 
                           (index == int_arr.size() - 1 ? "]" : ",");
                }
                out << "\n";
            } break;

            case JSONObject::JSONType::DOUBLE_ARR:
            {   
                out << "[";
                for (u64 index = 0; index < double_arr.size() ; ++index)
                {
                    out << double_arr[index] << 
                           (index == double_arr.size() - 1 ? "]" : ",");
                }
                out << "\n";
            } break;

            case JSONObject::JSONType::STR_ARR:
            {
                out << "[";
                for (u64 index = 0; index < str_arr.size() ; ++index)
                {
                    out << str_arr[index] << 
                           (index == str_arr.size() - 1 ? "]" : ",");
                }
                out << "\n";
            } break;

            case JSONObject::JSONType::OBJ_ARR:
            {
                out << "[\n";
                ++indent;
                for (u64 index = 0; index < obj_arr.size() ; ++index)
                {
                    out << std::string(indent, '\t') << "{\n";
                    obj_arr[index]->RecPrint(indent + 1, out);
                    out << std::string(indent, '\t') << "}";
                    out << (index != obj_arr.size() - 1 ? ",\n" : "\n");
                }
                out << std::string(--indent, '\t') << "]\n";
            } break;

            case JSONObject::JSONType::NUM_JSON_TYPES:
            {
            } break;
        }
}

void JSONObject::JSONValue::AssignValueByType(const JSONValue& src)
{
    switch (src.type)
    {   
        case JSONObject::JSONType::INT:
        {
            type = JSONType::INT;
            int_val = src.int_val;
        } break;

        case JSONObject::JSONType::DOUBLE:
        {
            type = JSONType::DOUBLE;
            double_val = src.double_val;
        } break;

        case JSONObject::JSONType::STR:
        {
            type = JSONType::STR;
            new (&str_val) std::string(src.str_val);
        } break;

        case JSONObject::JSONType::JSON_OBJECT:
        {
            type = JSONType::JSON_OBJECT;
            json_val = new JSONObject(*src.json_val);
        } break;

        case JSONObject::JSONType::INT_ARR:
        {
            type = JSONType::INT_ARR;
            new (&int_arr) std::vector<s32>(src.int_arr);
        } break;

        case JSONObject::JSONType::DOUBLE_ARR:
        {   
            type = JSONType::DOUBLE_ARR;
            new (&double_arr) std::vector<f64>(src.double_arr);
        } break;

        case JSONObject::JSONType::STR_ARR:
        {
            type = JSONType::STR_ARR;
            new (&str_arr) std::vector<std::string>(src.str_arr);
        } break;

        case JSONObject::JSONType::OBJ_ARR:
        {
            type = JSONType::OBJ_ARR;
            new (&obj_arr) std::vector<JSONObject*>();
            obj_arr.reserve(src.obj_arr.size());

            for (JSONObject* obj : src.obj_arr)
            {
                obj_arr.push_back(new JSONObject(*obj));
            }
        } break;

        case JSONObject::JSONType::NULL_TYPE:
        case JSONObject::JSONType::NUM_JSON_TYPES:
        {
            type = JSONType::NULL_TYPE;
        } break;
        
        case JSONObject::JSONType::BAD_TYPE:
        {
        } break;
    }
}


JSONObject::JSONValue::~JSONValue()
{
    switch (type)
    {
        case JSONType::NULL_TYPE:
        case JSONType::INT:
        case JSONType::DOUBLE:
        case JSONType::NUM_JSON_TYPES:
        {
            type = JSONType::NULL_TYPE;
        } break;

        case JSONType::STR:
        {
            str_val.~basic_string();
        } break;
        
        case JSONType::JSON_OBJECT:
        {
            delete json_val;
        } break;

        case JSONType::INT_ARR:
        {
            int_arr.~vector<s32>();
        } break;

        case JSONType::DOUBLE_ARR:
        {
            double_arr.~vector<f64>();
        } break;

        case JSONType::STR_ARR:
        {
            str_arr.~vector<std::string>();
        } break;

        case JSONType::OBJ_ARR:
        {
            for (JSONObject *obj : obj_arr)
            {
                delete obj;
            }
            obj_arr.~vector<JSONObject*>();
        } break;
        
        case JSONObject::JSONType::BAD_TYPE:
        {
        } break;
    }
}

JSONObject::JSONObject(const JSONObject& other) : insertion_order(other.insertion_order)
{
    for (auto& pair : other.json)
    {
        JSONValue *copy_value = new JSONValue(*(pair.second));
        json.insert({pair.first, copy_value});
    }
}

JSONObject& JSONObject::operator=(const JSONObject& other)
{
    if (this == &other)
    {
        return *this;
    }
    
    json.clear();
    json.insert(other.json.begin(), other.json.end());
   
    insertion_order.assign(other.insertion_order.begin(), other.insertion_order.end());

    return *this;
}

JSONObject::~JSONObject()
{
    // TODO(24.07.24): impl

    for (auto& pair : json)
    {
        delete pair.second;
    }
    json.clear();
    insertion_order.clear();
}


JSONObject& JSONObject::AddObj(const std::string &key)
{
    JSONObject *new_obj = new JSONObject();
    return *new_obj;
}   

void JSONObject::Remove(std::string key)
{
    // TODO(28.07.24): impl
}

JSONObject::JSONValue& JSONObject::operator[](std::string key)
{
    JSONIter value = json.find(key);
    if (value == json.end())
    {
        return bad_value;
    }
    return *(value->second);
}

std::ostream& operator<<(std::ostream& out, const JSONObject& obj)
{
    out << "{\n";
    obj.RecPrint(1, out);
    out << "}\n";

    return out;
}

std::ostream& operator<<(std::ostream& out, const JSONObject::JSONValue& value)
{
    value.PrintValueByType(0, out);
    return out;
}

void JSONObject::RecPrint(u8 indent, std::ostream& out) const
{
    for (std::string key : insertion_order)
    {
        const JSONObject::JSONValue* value = json.at(key);

        out << std::string(indent, '\t') << "\"" + key + "\": ";
        value->PrintValueByType(indent, out);
    }
}
