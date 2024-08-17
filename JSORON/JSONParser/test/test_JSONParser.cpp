/* ------------------------------------------*/ 
/* Filename: test_JSONParser.cpp             */
/* Date:     24.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include <vector>
#include <iostream>

#include "JSONParser.h"
#include "JSONObject.h"
#include "generic_test.h"

using namespace JSORON;

JSONObject simple_json1;
JSONObject simple_json2;
JSONObject simple_json3;

void InitSimpleJson1();
void InitSimpleJson2();
void InitSimpleJson3();

void TestLexer1(Tester& teser);
void TestLexer2(Tester& teser);
void TestLexer3(Tester& teser);

void TestParser1(Tester& teser);
void TestParser2(Tester& teser);
void TestParser3(Tester& teser);

void PrintTokenList(JSONParser::TokenList token_list);

int main(int argc, char *argv[])
{
    Tester tester;
    
    TestLexer1(tester);
    TestLexer2(tester);
    TestLexer3(tester);

    tester.TestAll();

	return 0;
}

void TestParser1(Tester& tester)
{

}

void TestParser2(Tester& tester)
{

}

void TestParser3(Tester& tester)
{

}

void TestLexer1(Tester& tester)
{
    JSONParser parser;
    parser.Lex("{ \"intKey\": 2 }");
    std::vector<JSONParser::Token> expected{JSONParser::Token('{'), 
                                            JSONParser::Token("intKey"), 
                                            JSONParser::Token(':'),
                                            JSONParser::Token(2),
                                            JSONParser::Token('}')};
    tester.TestPrimitive(parser.tokens, expected, "TestLexer1", __LINE__);
}


void TestLexer2(Tester& tester)
{
    JSONParser parser;
    parser.Lex("{ \"nestedJson\": {\"nestedInt\": 2}, \"intKey\": 42 }");
    std::vector<JSONParser::Token> expected{JSONParser::Token('{'), 
                                            JSONParser::Token("nestedJson"), 
                                            JSONParser::Token(':'),
                                            JSONParser::Token('{'),
                                            JSONParser::Token("nestedInt"),
                                            JSONParser::Token(':'),
                                            JSONParser::Token(2),
                                            JSONParser::Token('}'),
                                            JSONParser::Token(','),
                                            JSONParser::Token("intKey"), 
                                            JSONParser::Token(':'),
                                            JSONParser::Token(42),
                                            JSONParser::Token('}')};
    tester.TestPrimitive(parser.tokens, expected, "TestLexer2", __LINE__);
}

void TestLexer3(Tester& tester)
{
    JSONParser parser;
    parser.Lex("{ \"jsonArray\": [{\"json1\": 1},{\"json2\": 2}]}");
    std::vector<JSONParser::Token> expected{JSONParser::Token('{'), 
                                            JSONParser::Token("jsonArray"), 
                                            JSONParser::Token(':'),
                                            JSONParser::Token('['),
                                            JSONParser::Token('{'),
                                            JSONParser::Token("json1"), 
                                            JSONParser::Token(':'),
                                            JSONParser::Token(1),
                                            JSONParser::Token('}'),
                                            JSONParser::Token(','),
                                            JSONParser::Token('{'),
                                            JSONParser::Token("json2"), 
                                            JSONParser::Token(':'),
                                            JSONParser::Token(2),
                                            JSONParser::Token('}'),
                                            JSONParser::Token(']'),
                                            JSONParser::Token('}')};
    tester.TestPrimitive(parser.tokens, expected, "TestLexer3", __LINE__);
}

void InitSimpleJson1()
{
    simple_json1.Put("intKey", 2);
}

void InitSimpleJson2()
{
    JSONObject nested_json;
    nested_json.Put("intKey", 2);
    simple_json2.Put("nestedJson", nested_json);
}

void InitSimpleJson3()
{
    std::vector<JSONObject*> json_arr;
    
    JSONObject *json1 = new JSONObject();
    json1->Put("intKeyJson1", 1);
    JSONObject *json2 = new JSONObject();
    json2->Put("intKeyJson2", 2);

    json_arr.insert(json_arr.begin(), {json1, json2});

    simple_json3.Put("arrayOfJson", json_arr);
}

void PrintTokenList(JSONParser::TokenList token_list)
{
    std::cout << "[";

    for (JSONParser::Token tok : token_list)
    {
        switch (tok.type)
        {
            case JSONParser::TokenType::NULL_TYPE:
            case JSONParser::TokenType::NUM_TOKEN_TYPES:
            {
            } break;

            case JSONParser::TokenType::STR:
            {
                std::cout << tok.str_tok;
            } break;

            case JSONParser::TokenType::PANCTUATION:
            {
                std::cout << tok.panc_tok;
            } break;
            
            case JSONParser::TokenType::INT:
            {
                std::cout << tok.int_tok;
            } break;

            case JSONParser::TokenType::DOUBLE:
            {
                std::cout << tok.double_tok;
            } break;
        }
    }
    
    std::cout << "]\n";
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
