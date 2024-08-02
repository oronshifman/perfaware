/* ------------------------------------------*/ 
/* Filename: JSONObject_main.cpp           */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include "JSONObject.h"
#include "generic_test.h"

void TestObject(Tester& tester);
void TestObjectCopyCtor();
void TestOperatorSquereBrakets();
JSONObject CreateJson();

int main(int argc, char *argv[])
{
	Tester tester;
	TestObject(tester);
    TestOperatorSquereBrakets();

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

    return json;
}

void TestObject(Tester& tester)
{
    JSONObject json = CreateJson();

    std::cout << json;
}

void TestObjectCopyCtor()
{

}

void TestOperatorSquereBrakets()
{
    JSONObject json = CreateJson();

    std::cout << json["intKey"];
    std::cout << json["strKey"];
}




