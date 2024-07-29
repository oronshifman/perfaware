/* ------------------------------------------*/ 
/* Filename: JSORONObject_main.cpp           */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include "JSORONObject.h"
#include "generic_test.h"

void TestObject(Tester& tester);

int main(int argc, char *argv[])
{
	Tester tester;
	TestObject(tester);

	return 0;
}

void TestObject(Tester& tester)
{
	JSORONObject json;
	json.Put("intKey", 13);
	json.Put("doubleKey", 13.3);
	json.Put("strKey", "str");

	JSORONObject nested_jason;
	nested_jason.Put("nestedInt", 42);
	nested_jason.Put("nestedIntArr", std::vector<s32>{1,2,3});

	json.Put("nestedJson", nested_jason);

	u64 num_obj = 5;
	std::vector<JSORONObject*> json_arr;
	for (u64 index = 0; index < num_obj; ++index)
	{
		JSORONObject *json_obj = new JSORONObject();
		json_obj->Put("num", (s32)index);
		json_arr.push_back(json_obj);
	}	
	json.Put("ArrayOfJsons", json_arr);


	json.Print();
}


