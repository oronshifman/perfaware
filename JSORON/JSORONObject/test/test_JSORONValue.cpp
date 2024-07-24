/* ------------------------------------------*/ 
/* Filename: test_JSORONValue.cpp            */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include "JSORONValue.h"

#include "generic_test.h"

void TestIntVal(Tester& tester);
void TestFloatVal(Tester& tester);
void TestStrVal(Tester& tester);
void TestObjVal(Tester& tester);

void TestIntArr(Tester& tester);
void TestFloatArr(Tester& tester);
void TestStrArr(Tester& tester);
void TestObjArr(Tester& tester);

int main(int argc, char *argv[])
{
	Tester tester;

	TestIntVal(tester);
	TestFloatVal(tester);
	TestStrVal(tester);
	TestIntArr(tester);
	TestFloatArr(tester);
	TestStrArr(tester);

	tester.TestAll();

	return 0;
}

void TestIntVal(Tester& tester)
{
	int val = 13;
	JSORONValue json_val(val);

	tester.TestJSORONType(json_val.type, JSORONType::INT, "Int type", __LINE__);
	tester.TestPrimitive<int>(json_val.int_val, val, "Int value", __LINE__);
}

void TestFloatVal(Tester& tester)
{
	f64 val = 13.2;
	JSORONValue json_val(val);

	tester.TestJSORONType(json_val.type, JSORONType::FLOAT, "Float type", __LINE__);
	tester.TestPrimitive<float>(json_val.float_val, val, "Float value", __LINE__);
}

void TestStrVal(Tester& tester)
{
	std::string val = "hello";
	JSORONValue json_val(val);

	tester.TestJSORONType(json_val.type, JSORONType::STR, "Str type", __LINE__);
	tester.TestStr(json_val.str_val, val, "Str value", __LINE__);
}

void TestObjVal(Tester& tester)
{
	// TODO(24.07.24): JSORONObject not yet implemented
}

void TestIntArr(Tester& tester)
{
	std::vector<int> int_arr{1,2,3,4};
	JSORONValue json_val(int_arr);

	tester.TestJSORONType(json_val.type, JSORONType::INT_ARR, "Int array", __LINE__);
	for (int arr_index = 0, json_index = 0;
		 arr_index < int_arr.size();
		 ++arr_index, ++json_index)
	{
		tester.TestPrimitive<int>(int_arr[arr_index], json_val.int_arr[json_index], "Int Array: num in array", __LINE__);
	}
}

void TestFloatArr(Tester& tester)
{
	std::vector<f64> float_arr{1.2,2.2,3.2,4.2};
	JSORONValue json_val(float_arr);

	tester.TestJSORONType(json_val.type, JSORONType::FLOAT_ARR, "Float array", __LINE__);
	for (int arr_index = 0, json_index = 0;
		 arr_index < float_arr.size();
		 ++arr_index, ++json_index)
	{
		tester.TestPrimitive<f64>(float_arr[arr_index], json_val.float_arr[json_index], "Float Array: num in array", __LINE__);
	}
}

void TestStrArr(Tester& tester)
{
	std::vector<std::string> str_arr{"a","b","c","d"};
	JSORONValue json_val(str_arr);

	tester.TestJSORONType(json_val.type, JSORONType::STR_ARR, "Str array", __LINE__);
	for (int arr_index = 0, json_index = 0;
		 arr_index < str_arr.size();
		 ++arr_index, ++json_index)
	{
		tester.TestStr(str_arr[arr_index], json_val.str_arr[json_index], "Str Array: string in array", __LINE__);
	}
}

void TestObjArr(Tester& tester)
{
	// TODO(24.07.24): JSORONObject not yet implemented
}