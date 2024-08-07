/* ------------------------------------------*/ 
/* Filename: test_JSONParser.cpp             */
/* Date:     24.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include "JSONParser.h"

int main(int argc, char *argv[])
{

	return 0;
}

/*
{
    "intKey": 2
}

Things token should represent:
    string: brace, sqr_bracket, quotation, colon, comma
    int
    float

After lexing:
    ["{",""","intKey",""",":",2,"}"]

Thinking of parsing:
    have a stack for braces ordering
    for Token in TokenList
        if token == { && (stack.isEmpty() || stack.peek() == [)
            push to stack
        else
            exit with syntax error

        if token == " && (stack.peek() == { || stack.peek() == } || 
                          stack.peek() == [ || stack.peek() == ] ||)
            push to stack
        else
            exit with syntax error

        if token is a string
            key == token.string
                    

*/
