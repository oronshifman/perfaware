/* ------------------------------------------*/ 
/* Filename: JSORONParser.h                  */
/* Date:     22.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#ifndef __JSORON_PARSER_H__
#define __JSORON_PARSER_H__

#include <string>
#include <fstream>
#include <unordered_map>
#include <list>

#include "my_int.h"



template<typename V>
class JSORONObject;

template<typename V>
class JSORONValue
{
    JSORONType type;
    V value;

    JSORONValue(std::string type, V value) : type(type), value(value) {}
    
    friend class JSORONObject;

    template<typename U>  
    friend std::ostream &operator<<(std::ostream &out, const JSORONObject<U> &json_obj);

    template<typename U>  
    friend void Print(std::ostream &out, const JSORONObject<U> &json_obj, u8 indentations);
};

template<typename V>
class JSORONObject
{
public:
    JSORONObject()
    {
        json = new std::unordered_map<std::string, JSORONValue<V>>;
    }

    ~JSORONObject()
    {
        delete json;
    }

    V Get(std::string key)
    {
        return json[key]->value;
    }

    void Put(std::string key, V value)
    {
        auto& ret = json->insert(key, value);
        if (ret.second == true)
        {
            insertion_order.push_back(key);
        }
    }

    auto& operator[](std::string key)
    {
        return *json[key];
    }

private:
    std::unordered_map<std::string, JSORONValue<V>> *json;
    std::list<std::string> insertion_order;
    
    template<typename U>  
    friend std::ostream &operator<<(std::ostream &out, const JSORONObject<U> &json_obj);

    template<typename U>  
    friend void Print(std::ostream &out, const JSORONObject<U> &json_obj, u8 indentations);
};

// template<typename T>
// class JSORONParser 
// {
// public:
//     JSORONObject &Parse(std::string json_str)
//     {
//         JSORONObject new_json;
//         for (auto &iter_c = json_str.begin(); iter_c != json_str.end(); ++iter_c)
//         {
//             if (*iter_c == '{')
//             {
//                 std::string new_key;
//                 while (!std::isalnum(*iter_c));
//                 while (*iter_c != '"')
//                 {
//                     new_key += *iter_c;
//                 }
//                 new_json.Put(new_key, NULL)
//             }
//         }
//     }
//     JSORONObject &Parse(std::ifstream json_file);
// };

template<typename V>  
std::ostream &operator<<(std::ostream &out, const JSORONObject<V> &json_obj)
{
    out << "{" << std::endl;

    Print(out, json_obj, 1);
}

template<typename V>  
void Print(std::ostream &out, const JSORONObject<V> &json_obj, u8 indentations)
{
    for (std::string key : json_obj.insertion_order)
    {
        JSORONValue& value = json_obj[key].type;
        switch (value.type)
        {
            case JSORONType::ARRAY:
            {
                for ()
            } break;

            case JSORONType::JSORONObject:
            {

            } break;

            case JSORONType::STRING:
            {

            } break;

            case JSORONType::INT:
            {

            } break;
            
            case JSORONType::FLOAT:
            {

            } break;
        }
    }
}
#endif /* JSORON_PARSER_H */

























