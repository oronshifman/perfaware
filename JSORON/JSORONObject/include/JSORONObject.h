/* ------------------------------------------*/ 
/* Filename: JSORONObject.h                  */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#ifndef __JSORON_OBJECT_H__
#define __JSORON_OBJECT_H__

#include <unordered_map>
#include <list>
#include <string>
#include <vector>

#include "my_int.h"

class JSORONObject 
{
    enum class JSORONType
    {
        NULL_TYPE,

        INT,
        DOUBLE,
        STR,
        JSORON_OBJECT,

        INT_ARR,
        DOUBLE_ARR,
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
            f64 double_val;
            std::string str_val;
            JSORONObject *json_val;

            std::vector<s32> int_arr;
            std::vector<f64> double_arr;
            std::vector<std::string> str_arr;
            std::vector<JSORONObject*> obj_arr;
        };

        JSORONValue() : type(JSORONType::NULL_TYPE) {}
        JSORONValue(const JSORONValue& value);
        ~JSORONValue();

        JSORONValue(const s32 value) : type(JSORONType::INT), int_val(value) {}
        JSORONValue(const f64 value) : type(JSORONType::DOUBLE), double_val(value) {}
        JSORONValue(const std::string value) : type(JSORONType::STR), str_val(value) {}
        JSORONValue(const JSORONObject &value);

        JSORONValue(const std::vector<s32>& arr) : type(JSORONType::INT_ARR), int_arr(arr) {}
        JSORONValue(const std::vector<f64>& arr) : type(JSORONType::DOUBLE_ARR), double_arr(arr) {}
        JSORONValue(const std::vector<std::string>& arr) : type(JSORONType::STR_ARR), str_arr(arr) {}
        JSORONValue(const std::vector<JSORONObject*>& arr) : type(JSORONType::OBJ_ARR), obj_arr(arr) {}
    };

public:
    JSORONObject() : json(), insertion_order() {}
    JSORONObject(const JSORONObject& value);
    ~JSORONObject();

    template<typename T>
    void Put(const std::string& key, const T& value)
    {
        JSORONValue new_value(value);
        auto res = json.insert({key, new_value});
        if (res.second)
        {
            insertion_order.push_back(key);
        }
    }
    // void Put(const std::string& key, const f64 value);
    // void Put(const std::string& key, const std::string value);
    // void Put(const std::string& key, const JSORONObject& value);
    // void Put(const std::string& key, const std::vector<s32>& value);
    // void Put(const std::string& key, const std::vector<f64>& value);
    // void Put(const std::string& key, const std::vector<std::string>& value);
    // void Put(const std::string& key, const std::vector<JSORONObject>& value);

    /**
     * @brief adds a new json object to this json
     * @param key the key for the new json object
     * @return a reference to the new object that was inserted
     */
    JSORONObject& AddObj(const std::string& key);
    
    /**
     * @brief adds a new array of type T to this json
     * @param key the key for the new array
     * @return a reference to the new array that was inserted
     */
    template<typename T>
    std::vector<T>& AddArr(const std::string& key)
    {
        std::vector<T> new_arr;
        JSORONValue new_value(new_arr);
        auto res = json.insert({key, new_value});
        if (res.second)
        {
            insertion_order.push_back(key);
        }
        return res.first->second;
    }

    void Remove(std::string key);

    friend std::ostream& operator<<(std::ostream& out, const JSORONObject obj);

private:
    std::unordered_map<std::string, JSORONValue> json;
    std::list<std::string> insertion_order;
};

#endif /* JSORON_OBJECT_H */

