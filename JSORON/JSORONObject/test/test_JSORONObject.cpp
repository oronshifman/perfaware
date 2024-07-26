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
	// json.Put("doubleKey", 13.3);
	// json.Put("strKey", "str");

	std::cout << json;
}


