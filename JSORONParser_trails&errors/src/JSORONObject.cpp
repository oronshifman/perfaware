/* ------------------------------------------*/ 
/* Filename: JSORONObject.cpp                */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include "JSORONObject.h"

JSORONObject::JSORONObject()
{
    json = new std::unordered_map<std::string, JSORONValue>;
    insertion_order = new std::list<std::string>;
}

JSORONObject::~JSORONObject()
{
    delete json;
    delete insertion_order;
}

JSORONObject::JSORONObject(const JSORONObject &src)
{
    json = src.json;
    insertion_order = src.insertion_order;
}

JSORONObject& JSORONObject::operator=(const JSORONObject &src)
{
    this->~JSORONObject();
    json = src.json;
    insertion_order = src.insertion_order;
 
    return *this;
}

JSORONValue& JSORONObject::Get(std::string key)
{
    return (*this)[key];
}

void JSORONObject::PutInt(std::string key, u32 value)
{
    using iterator = std::unordered_map<std::string, JSORONValue>::iterator;

    JSORONValue new_value;
    new_value.type = JSORONType::INT;
    new_value.int_value = value;

    std::pair<iterator, bool> ret = json->insert({key, new_value});
    if (ret.second)
    {
        insertion_order->push_back(key);
    }
}

void JSORONObject::PutStr(std::string key, std::string value)
{
    auto iter = json->find(key);
    if (iter != json->end())
    {
        return;
    }

    JSORONValue new_value;
    new_value.type = JSORONType::STRING;
    new_value.str_value = value;

    auto ret = json->insert({key, new_value});
    if (ret.second)
    {
        insertion_order->push_back(key);
    }
}

void JSORONObject::PutFloat(std::string key, f64 value)
{
    auto iter = json->find(key);
    if (iter != json->end())
    {
        return;
    }

    JSORONValue new_value;
    new_value.type = JSORONType::FLOAT;
    new_value.float_value = value;

    auto ret = json->insert({key, new_value});
    if (ret.second)
    {
        insertion_order->push_back(key);
    }
}

void JSORONObject::PutJsonObject(std::string key, JSORONObject value)
{
    // NOTE(23.7.24): stab
    // using iterator = std::unordered_map<std::string, JSORONValue>::iterator;

    // JSORONValue new_value;
    // new_value.type = JSORONType::JSORON_OBJECT;
    // new_value.json_value = value;

    // std::pair<iterator, bool> ret = json->insert({key, new_value});
    // if (ret.second == true)
    // {
    //     insertion_order->push_back(key);
    // }
}

void JSORONObject::PutJsonArray(std::string key, JSORONArray value)
{
    // NOTE(23.7.24): stab
}

JSORONValue& JSORONObject::operator[](const std::string &key)
{
    auto iter = json->find(key);
    if (iter != json->end())
    {
        return iter->second;
    }

    JSORONValue new_value;
    new_value.type = JSORONType::NULL_TYPE;

    auto ret = json->insert({key, new_value});
    if (ret.second)
    {
        insertion_order->push_back(key);
    }

    return ret.first->second;
}

JSORONValue::JSORONValue(const JSORONValue &src)
{
    type = src.type;
    switch (type)
    {
        case JSORONType::ARRAY:
        {
            // TODO: impl
        } break;

        case JSORONType::JSORON_OBJECT:
        {
            json_value = src.json_value; // NOTE(23.7.24): not tested!
        } break;

        case JSORONType::STRING:
        {
            str_value = src.str_value;
        } break;
        
        case JSORONType::INT:
        {
            int_value = int_value;
        } break;

        case JSORONType::FLOAT:
        {
            float_value = float_value;
        } break;
    }
}
