/* ------------------------------------------*/ 
/* Filename: JSONObject_main.cpp           */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include "JSONObject.h"
#include "generic_test.h"

using namespace JSORON;

JSONObject CreateJson();

void TestObjectCopyCtor(Tester& tester);
void TestObjectCopyAssignment(Tester& tester);
void TestJSONValueCopyAssignment(Tester& tester);
void TestOperatorSquereBrakets(Tester& tester);
void TestJSONValueCasting(Tester& tester);

int main(int argc, char *argv[])
{
	Tester tester;
    
    TestOperatorSquereBrakets(tester);

    TestObjectCopyCtor(tester);

    TestObjectCopyAssignment(tester);

    TestJSONValueCopyAssignment(tester);
    
    TestJSONValueCasting(tester);

    tester.TestAll();

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

void TestObjectCopyCtor(Tester& tester)
{
    JSONObject json1 = CreateJson();
    JSONObject json2(json1);

    tester.TestPrimitive(json2, json1, "TestObjectCopyCtor", __LINE__);
}

void TestObjectCopyAssignment(Tester& tester)
{
    JSONObject json1 = CreateJson();
    JSONObject json2 = json1;

    tester.TestPrimitive(json2, json1, "TestObjectCopyCtor", __LINE__);
}

void TestJSONValueCopyAssignment(Tester& tester)
{
    JSONObject json = CreateJson();

    json["intKey"] = 42;
    tester.TestPrimitive(json["intKey"], JSONObject::JSONValue(42), "TestJSONValueCopyAssignment", __LINE__);

    json["strKey"] = json["strKey"];
    tester.TestPrimitive(json["intKey"], JSONObject::JSONValue(42), "TestJSONValueCopyAssignment", __LINE__);
    
    json["intKey"] = "not an int!";
    tester.TestPrimitive(json["intKey"], JSONObject::JSONValue("not an int!"), "TestJSONValueCopyAssignment", __LINE__);

    json["intKey"] = json["nestedJson"];
    tester.TestPrimitive(json["intKey"], JSONObject::JSONValue(json["nestedJson"]), "TestJSONValueCopyAssignment", __LINE__);
}

void TestOperatorSquereBrakets(Tester& tester)
{
    JSONObject json = CreateJson();

    tester.TestPrimitive(JSONObject::JSONValue(13), JSONObject::JSONValue(json["intKey"]), "TestOperatorSquereBrakets", __LINE__);
    tester.TestPrimitive(JSONObject::JSONValue("str"), JSONObject::JSONValue(json["strKey"]), "TestOperatorSquereBrakets", __LINE__);
}

void TestJSONValueCasting(Tester& tester)
{
    JSONObject json = CreateJson();

    s32 int_val = json["intKey"];
    std::vector<JSONObject*> array_of_jsons = json["ArrayOfJsons"];

    tester.TestPrimitive(int_val, 13, "TestJSONValueCasting", __LINE__);

    for (auto new_iter = array_of_jsons.begin(), og_iter = json["ArrayOfJsons"].obj_arr.begin();
         new_iter != array_of_jsons.end() && og_iter != json["ArrayOfJsons"].obj_arr.end();
         ++new_iter, ++og_iter)
    {
        tester.TestPrimitive(*new_iter, *og_iter, "TestJSONValueCasting", __LINE__);
    }
}



















