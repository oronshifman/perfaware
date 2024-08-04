/* ------------------------------------------*/ 
/* Filename: JSONObject_main.cpp           */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include "JSONObject.h"
#include "generic_test.h"

void TestObject(Tester& tester);
void TestObjectCopyCtor();
void TestObjectCopyAssignment();
void TestJSONValueCopyAssignment();
void TestOperatorSquereBrakets();
JSONObject CreateJson();

int main(int argc, char *argv[])
{
	Tester tester;

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
    JSONObject json1 = CreateJson();
    JSONObject json2(json1);

    std::cout << json2;
}

void TestObjectCopyAssignment()
{
    // TODO(3.8.24): finish implementing test
    JSONObject json1 = CreateJson();
    JSONObject json2 = json1;

    std::cout << json2;
}

void TestJSONValueCopyAssignment()
{
    // TODO(3.8.24): implement test
    JSONObject json = CreateJson();

    json["intKey"] = 42;
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




