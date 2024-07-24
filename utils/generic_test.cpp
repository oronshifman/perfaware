#include <stdio.h>

#include "generic_test.h"

Tester::Tester()
{
    test_counter = 0;
    succeeded_test_counter = 0;
}

void Tester::TestJSORONType(JSORONType result, JSORONType expected, std::string message, int line)
{
    ++test_counter;
    
    if (result != expected)
    {
        std::cout << "\n" << BOLDMAGENTA << "Test " << std::to_string(test_counter) << DEFAULT << "\n" <<
                    message << "\n" << 
                    BOLDRED << "FAIL " << DEFAULT <<
                    "at line " << BOLDBLUE << std::to_string(line) << DEFAULT << "\n" <<
                    "Got: " << RED << (int)result << DEFAULT << 
                    " expected: " << GREEN << (int)expected << "\n";
    }
    else
    {
        ++succeeded_test_counter;
    }	
}

void Tester::TestStr(std::string result, std::string expected, std::string message, int line)
{
    ++test_counter;
    
    if (result != expected)
    {
        std::cout << "\n" << BOLDMAGENTA << "Test " << std::to_string(test_counter) << DEFAULT << "\n" <<
                    message << "\n" << 
                    BOLDRED << "FAIL " << DEFAULT <<
                    "at line " << BOLDBLUE << std::to_string(line) << DEFAULT << "\n" <<
                    "Got: " << RED << result << DEFAULT << 
                    " expected: " << GREEN << expected << "\n";
    }
    else
    {
        ++succeeded_test_counter;
    }	
}

void Tester::TestAll(void)
{
	if (test_counter == succeeded_test_counter)
	{
		printf(DEFAULT "\nAll %lu tests " BOLDGREEN "SUCCESS\n" DEFAULT, test_counter);
	}
	else
	{
		printf(DEFAULT "\n%lu out of %lu tests " BOLDRED "FAIL\n" DEFAULT, 
			   test_counter - succeeded_test_counter, test_counter);
	}
}