/* ------------------------------------------*/ 
/* Filename: JSONParser.cpp                  */
/* Date:     24.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include <fstream>
#include <string>
#include <unordered_set>

#include "JSONParser.h"
#include "JSONObject.h"
#include "my_int.h"

namespace JSORON
{

JSONParser::Token::~Token()
{
    switch(type)
    {
        case JSONParser::TokenType::NULL_TYPE:
        case JSONParser::TokenType::INT:
        case JSONParser::TokenType::DOUBLE:
        case JSONParser::TokenType::NUM_TOKEN_TYPES:
        {
        } break;

        case JSONParser::TokenType::PANCTIOATION:
        case JSONParser::TokenType::STR:
        {
            str_tok.~basic_string();
        } break;
    }
}

const JSONObject JSONParser::bad_obj = JSONObject();

JSONObject JSONParser::Parse(const std::string& json_str)
{
    // TODO(7.8.24): impl
    return bad_obj;
}

JSONObject JSONParser::Parse(const std::ifstream& json_file)
{
    // TODO(7.8.24): impl
    return bad_obj;
}

static void SyntaxError(std::string where, char expected, char got)
{
    fprintf(stderr, "Syntax error: After %s, got: %c, expected: %c", where.c_str(), got, expected);
}

JSONParser::TokenList JSONParser::Lex(std::string json_str)
{
    // TODO(7.8.24): impl
    while (!json_str.empty())
    {
        if (IsPanctioation(json_str[0]))
        {
            LexPanctioation(json_str[0]);
            json_str.erase(json_str.begin());
            continue;
        }

        if (tokens.back().type == TokenType::PANCTIOATION &&
            tokens.back().panc_tok == '"')
        {
            LexString(json_str);
            json_str.erase(0,
                            tokens.back().str_tok.size() + 1);
            continue;
        }
           
    }
    return JSONParser::TokenList();
}

void JSONParser::LexPanctioation(const char panc)
{
    switch (panc)
    {
        case '{':
        {
            tokens.push_back(Token('{'));
        } break;

        
        case '}':
        {
            tokens.push_back(Token('}'));
        } break;


        case '[':
        {
            tokens.push_back(Token('['));
        } break;

        case ']':
        {
            tokens.push_back(Token(']'));
        } break;

        case '"':
        {
            tokens.push_back(Token('"'));
        } break;
    }
}

void JSONParser::LexString(const std::string& json_str)
{
    std::string str;
    str.insert(json_str.begin(), json_str.find('"'));
    tokens.push_back(Token(str));
}

b8 JSONParser::IsPanctioation(const char panc)
{
    static std::unordered_set<char> panctioations{'{','}','[',']','"',':',','};
    
    auto iter = panctioations.find(panc);
    if (iter != panctioations.end())
    {
        return 1;
    }

    return 0;
}

} // namespace JSORON



















