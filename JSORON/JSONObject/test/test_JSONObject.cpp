/* ------------------------------------------*/ 
/* Filename: JSONObject_main.cpp           */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include "JSONObject.h"
#include "generic_test.h"

using namespace JSORON;

void TestMemoryManagment();

void TestObject(Tester& tester);
void TestObjectCopyCtor();
void TestObjectCopyAssignment();
void TestJSONValueCopyAssignment();
void TestOperatorSquereBrakets();
void TestJSONValueCasting();
JSONObject CreateJson();

int main(int argc, char *argv[])
{
	Tester tester;
    
    TestMemoryManagment();

    std::cout << "TestObject:\n";
	TestObject(tester);

    std::cout << "\nTestOperatorSquereBrakets:\n";
    TestOperatorSquereBrakets();

    std::cout << "\nTestObjectCopyCtor:\n";
    TestObjectCopyCtor();

    std::cout << "\nTestObjectCopyAssignment:\n";
    TestObjectCopyAssignment();

    std::cout << "\nTestJSONValueCopyAssignment:\n";
    TestJSONValueCopyAssignment();
    
    std::cout << "\n TetstJSONValueCastring\n";
    TestJSONValueCasting();

	return 0;
}

JSONObject CreateJson()
{
	JSONObject json;

    json.Put("intKey", 13);
	json.Put("doubleKey", 13.3);
	json.Put("strKey", "str");

	JSONObject nested_jason;
	nested_jason.Put("nestedInt", 42);
	nested_jason.Put("nestedIntArr", std::vector<s32>{1,2,3});

	json.Put("nestedJson", nested_jason);

	u64 num_obj = 5;
	std::vector<JSONObject*> json_arr;
	for (u64 index = 0; index < num_obj; ++index)
	{
		JSONObject *json_obj = new JSONObject();
		json_obj->Put("num", (s32)index);
		json_arr.push_back(json_obj);
	}	
	json.Put("ArrayOfJsons", json_arr);
    for (auto obj : json_arr)
    {
        delete obj;
    }

    return json;
}

void TestMemoryManagment()
{
    JSONObject json;
    
	JSONObject nested_jason;
	nested_jason.Put("nestedInt", 42);
	nested_jason.Put("nestedIntArr", std::vector<s32>{1,2,3});

    json.Put("nestedJson", nested_jason);

    std::cout << json;
}

void TestObject(Tester& tester)
{
    JSONObject json = CreateJson();

    std::cout << json;
}

void TestObjectCopyCtor()
{
    JSONObject json1 = CreateJson();
    JSONObject json2(json1);

    std::cout << json2;
}

void TestObjectCopyAssignment()
{
    JSONObject json1 = CreateJson();
    JSONObject json2 = json1;

    std::cout << json2;
}

void TestJSONValueCopyAssignment()
{
    JSONObject json = CreateJson();

    json["intKey"] = 42;
    std::cout << json;

    json["strKey"] = json["strKey"];
    std::cout << json;
    
    json["intKey"] = "not an int!";
    std::cout << json;

    json["intKey"] = json["nestedJson"];
    std::cout << json;
}

void TestOperatorSquereBrakets()
{
    JSONObject json = CreateJson();

    std::cout << json["intKey"];
    std::cout << json["strKey"];
}

void TestJSONValueCasting()
{
    JSONObject json = CreateJson();

    s32 int_val = json["intKey"];
    std::vector<JSONObject*> array_of_jsons = json["ArrayOfJsons"];

    std::cout << "this is intkey's value: " << int_val << "\n";
    std::cout << "this is ArrayOfJsons's value:\n" << array_of_jsons;
}



















