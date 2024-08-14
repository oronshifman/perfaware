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


namespace JSORON
{
    class JSONObject 
    {
    #ifndef DNDEBUG
    public: 
    #endif /* DNDEBUG */
        enum class ValueType
        {
            BAD_TYPE,
            NULL_TYPE,
    
            KEY,
            
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
            ValueType type;
    
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
    
            JSONValue() : type(ValueType::NULL_TYPE) {}
            JSONValue(const JSONValue& value);
            JSONValue& operator=(const JSONValue& other);
            
            template<typename T>
            JSONValue& operator=(const T& src);
    
            ~JSONValue();
    
            JSONValue(const ValueType& type) : type(type) {}
    
            JSONValue(const ValueType type, const std::string key) : type(type), str_val(key) {}
            JSONValue(const s32 value) : type(ValueType::INT), int_val(value) {}
            JSONValue(const f64 value) : type(ValueType::DOUBLE), double_val(value) {}
            JSONValue(const std::string value) : type(ValueType::STR), str_val(value) {}
            JSONValue(const JSONObject &value);
    
            JSONValue(const std::vector<s32>& arr) : type(ValueType::INT_ARR), int_arr(arr) {}
            JSONValue(const std::vector<f64>& arr) : type(ValueType::DOUBLE_ARR), double_arr(arr) {}
            JSONValue(const std::vector<std::string>& arr) : type(ValueType::STR_ARR), str_arr(arr) {}
            JSONValue(const std::vector<JSONObject*>& arr);
    
            /**
             * @brief overloading cast to int.
             * @throw bad_cast
             */
            operator int() const;
            
            /**
             * @brief overloading cast to double.
             * @throw bad_cast
             */
            operator double() const;
           
            /**
             * @brief overloading cast to string.
             * @throw bad_cast
             */
            operator std::string() const;
          
            /**
             * @brief overloading cast to JSONObject.
             * @throw bad_cast
             */
            operator JSONObject*() const;
         
            /**
             * @brief overloading cast to std::vector<int>.
             * @throw bad_cast
             */
            operator std::vector<int>() const;
        
            /**
             * @brief overloading cast to std::vector<double>.
             * @throw bad_cast
             */
            operator std::vector<double>() const;
       
            /**
             * @brief overloading cast to std::vector<std::string>.
             * @throw bad_cast
             */
            operator std::vector<std::string>() const;
      
            /**
             * @brief overloading cast to std::vector<JSONObject*>.
             * @throw bad_cast
             */
            operator std::vector<JSONObject*>() const;
            
            void PrintValueByType(u8 indent, std::ostream& out) const;
            void AssignValueByType(const JSONValue& src);
    
            friend std::ostream& operator<<(std::ostream& out, const JSONValue& value);
        };
    
    public:
        JSONObject() : json(), insertion_order() {}
        JSONObject(const JSONObject& other);
        JSONObject& operator=(const JSONObject& obj);
        ~JSONObject();
    
        template<typename T>
        void Put(const std::string key, const T& value);
    
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
        std::vector<T>& AddArr(const std::string& key);
    
        void Remove(std::string key);
        
        /**
         * @brief access values in json object
         * @param key - the key associated with the value to be pulled from the json object
         * @return if key exists in json object, returns a reference the value associated with 
         *         key else return a reference to a JSONValue of type ValueType::NULL_TYPE
         */
        JSONValue& operator[](std::string key);
        
        friend class JSONParser;

        friend std::ostream& operator<<(std::ostream& out, const JSONObject& obj);
        friend std::ostream& operator<<(std::ostream& out, const JSONValue& value);
    
    private:
        typedef std::unordered_map<std::string, JSONValue*>::iterator JSONIter;
    
        static JSONValue bad_value;
        std::unordered_map<std::string, JSONValue*> json;
        std::list<std::string> insertion_order;
    
        void RecPrint(u8 indent, std::ostream& out) const;
    };
    
    template<typename T>
    JSONObject::JSONValue& JSONObject::JSONValue::operator=(const T& src)
    {
        this->~JSONValue();
    
        JSONValue *new_value = new JSONValue(src);
        AssignValueByType(*new_value);
        delete new_value;
    
        return *this;
    }
    
    template<typename T>
    void JSONObject::Put(const std::string key, const T& value)
    {
        JSONValue* new_value = new JSONValue(value);
        auto res = json.insert({key, new_value});
        if (res.second)
        {
            insertion_order.push_back(key);
        }
    }
    
    template<typename T>
    std::vector<T>& JSONObject::AddArr(const std::string& key)
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
}

#endif /* JSON_OBJECT_H */

