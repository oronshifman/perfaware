/* ------------------------------------------*/ 
/* Filename: JSORONObject.h                  */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#ifndef __JSORON_OBJECT_H__
#define __JSORON_OBJECT_H__

#include <string>
#include <unordered_map>
#include <list>

#include "my_int.h"
#include "JSORONArray.h"

class JSORONObject;

namespace JSORONType
{
    enum JSORONType
    {
        NULL_TYPE,
        ARRAY, // NOTE: might be a different class altogether
        JSORON_OBJECT,
        STRING,
        FLOAT,
        INT,

        NUM_JSORONTypes    
    };
} // JSORONType

struct JSORONValue 
{
    JSORONValue() {}
    ~JSORONValue() {}
    JSORONValue(const JSORONValue& src);

    JSORONType::JSORONType type;

    union
    {
        std::string str_value;   
        JSORONObject *json_value;
        u32 int_value;
        f64 float_value;
    };  
};

class JSORONObject 
{
public:
    JSORONObject();
    ~JSORONObject();
    JSORONObject(const JSORONObject& src);
    JSORONObject& operator=(const JSORONObject& src);
   
    JSORONValue& Get(std::string key);

    void PutInt(std::string key, u32 value);
    void PutStr(std::string key, std::string value);
    void PutFloat(std::string key, f64 value);
    void PutJsonObject(std::string key, JSORONObject value);
    void PutJsonArray(std::string key, JSORONArray value);

    JSORONValue& operator[](const std::string& key);

private:
    std::unordered_map<std::string, JSORONValue> *json;
    std::list<std::string> *insertion_order;
};

#endif /* JSORON_OBJECT_H */
