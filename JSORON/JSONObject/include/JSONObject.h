/* ------------------------------------------*/ 
/* Filename: JSONObject.h                    */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#ifndef __JSON_OBJECT_H__
#define __JSON_OBJECT_H__

#include <ostream>
#include <unordered_map>
#include <list>
#include <string>
#include <vector>

#include "my_int.h"

class JSONObject 
{
    enum class JSONType
    {
        NULL_TYPE,

        INT,
        DOUBLE,
        STR,
        JSON_OBJECT,

        INT_ARR,
        DOUBLE_ARR,
        STR_ARR,
        OBJ_ARR,

        NUM_JSON_TYPES
    };
    
    class JSONValue 
    {
    public:
        JSONType type;

        union
        {
            s32 int_val;
            f64 double_val;
            std::string str_val;
            JSONObject *json_val;

            std::vector<s32> int_arr;
            std::vector<f64> double_arr;
            std::vector<std::string> str_arr;
            std::vector<JSONObject*> obj_arr;
        };

        JSONValue() : type(JSONType::NULL_TYPE) {}
        JSONValue(const JSONValue& value);
        ~JSONValue();

        JSONValue(const s32 value) : type(JSONType::INT), int_val(value) {}
        JSONValue(const f64 value) : type(JSONType::DOUBLE), double_val(value) {}
        JSONValue(const std::string value) : type(JSONType::STR), str_val(value) {}
        JSONValue(const JSONObject &value);

        JSONValue(const std::vector<s32>& arr) : type(JSONType::INT_ARR), int_arr(arr) {}
        JSONValue(const std::vector<f64>& arr) : type(JSONType::DOUBLE_ARR), double_arr(arr) {}
        JSONValue(const std::vector<std::string>& arr) : type(JSONType::STR_ARR), str_arr(arr) {}
        JSONValue(const std::vector<JSONObject*>& arr) : type(JSONType::OBJ_ARR), obj_arr(arr) {}
        /**
         * @brief overloding cast to int.
         * @throw bad_cast
         */
        operator int() const;
        
        /**
         * @brief overloding cast to double.
         * @throw bad_cast
         */
        operator double() const;
       
        /**
         * @brief overloding cast to string.
         * @throw bad_cast
         */
        operator std::string() const;
      
        /**
         * @brief overloding cast to JSONObject.
         * @throw bad_cast
         */
        operator JSONObject*() const;
     
        /**
         * @brief overloding cast to std::vector<int>.
         * @throw bad_cast
         */
        operator std::vector<int>() const;
    
        /**
         * @brief overloding cast to std::vector<double>.
         * @throw bad_cast
         */
        operator std::vector<double>() const;
   
        /**
         * @brief overloding cast to std::vector<std::string>.
         * @throw bad_cast
         */
        operator std::vector<std::string>() const;
  
        /**
         * @brief overloding cast to std::vector<JSONObject*>.
         * @throw bad_cast
         */
        operator std::vector<JSONObject*>() const;
        
        void PrintValueByType(u8 indent, std::ostream& out, JSONType type) const;

        friend std::ostream& operator<<(std::ostream& out, const JSONValue *value);

        static const std::string JSONType_to_string[static_cast<u64>(JSONObject::JSONType::NUM_JSON_TYPES)];
    };

public:
    JSONObject() : json(), insertion_order() {}
    JSONObject(const JSONObject& value) : json(value.json), insertion_order(value.insertion_order) {}
    JSONObject& operator=(const JSONObject& obj);
    ~JSONObject();

    template<typename T>
    void Put(const std::string key, const T& value)
    {
        JSONValue* new_value = new JSONValue(value);
        auto res = json.insert({key, new_value});
        if (res.second)
        {
            insertion_order.push_back(key);
        }
    }

    /**
     * @brief adds a new json object to this json
     * @param key the key for the new json object
     * @return a reference to the new object that was inserted
     */
    JSONObject& AddObj(const std::string& key);
    
    /**
     * @brief adds a new array of type T to this json
     * @param key the key for the new array
     * @return a reference to the new array that was inserted
     */
    template<typename T>
    std::vector<T>& AddArr(const std::string& key)
    {
        std::vector<T> new_arr;
        JSONValue* new_value = new JSONValue(new_arr);
        auto res = json.insert({key, new_value});
        if (res.second)
        {
            insertion_order.push_back(key);
        }
        return res.first->second;
    }

    void Remove(std::string key);
    
    JSONValue *operator[](std::string key);
    
    friend std::ostream& operator<<(std::ostream& out, const JSONObject& obj);
    friend std::ostream& operator<<(std::ostream& out, const JSONValue *value);

private:
    typedef std::unordered_map<std::string, JSONValue*>::iterator JSONIter;

    std::unordered_map<std::string, JSONValue*> json;
    std::list<std::string> insertion_order;

    void RecPrint(u8 indent, std::ostream& out) const;
};

#endif /* JSON_OBJECT_H */

