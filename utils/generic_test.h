/**********************
 Author:	oron
 Date:		24.7.24	
************************/

#ifndef __GENERIC_TEST_H__
#define __GENERIC_TEST_H__

#include <iostream>
#include <string>
#include <stddef.h> // size_t

/*************************************************
                  Color Defines   		                        
*************************************************/
#define DEFAULT     "\033[0m"
#define BLACK       "\033[30m"             /* Black */
#define RED         "\033[31m"             /* Red */
#define GREEN       "\033[32m"             /* Green */
#define YELLOW      "\033[33m"             /* Yellow */
#define BLUE        "\033[34m"             /* Blue */
#define MAGENTA     "\033[35m"             /* Magenta */
#define CYAN        "\033[36m"             /* Cyan */
#define WHITE       "\033[37m"             /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

/**
 * Create an instance of Tester and the call it's test function as discribed above it
 */
class Tester {
public:
    Tester();

    /**
     * Call: Test<tested_type>(result, expected, "", __LINE__); 
     */
    template<typename T>
    void TestPrimitive(T result, T expected, std::string message, int line)
    {
        ++test_counter;
        
        if (result != expected)
        {
            std::cout << "\n" << BOLDMAGENTA << "Test " << std::to_string(test_counter) << DEFAULT << "\n" <<
                        message << "\n" << 
                        BOLDRED << "FAIL " << DEFAULT <<
                        "at line " << BOLDBLUE << std::to_string(line) << DEFAULT << "\n" <<
                        "Got: " << RED << std::to_string(result) << DEFAULT << 
                        " expected: " << GREEN << std::to_string(expected) << "\n";
        }
        else
        {
            ++succeeded_test_counter;
        }	
    }

    void TestStr(std::string result, std::string expected, std::string message, int line);
    void TestAll(void);

private:
    size_t test_counter;
    size_t succeeded_test_counter;
};

#endif /* GENERIC_TEST_H */
