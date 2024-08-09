/* ------------------------------------------*/ 
/* Filename: JSONParser.cpp                  */
/* Date:     24.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include <fstream>
#include <string>
#include <unordered_set>
#include <algorithm>

#include "JSONParser.h"
#include "JSONObject.h"
#include "my_int.h"

namespace JSORON
{
    JSONParser::Token::Token(const Token& other) : type(other.type)
    {
        switch (other.type)
        {
            case JSONParser::TokenType::NULL_TYPE:
            case JSONParser::TokenType::NUM_TOKEN_TYPES:
            {
            } break;

            case JSONParser::TokenType::STR:
            {
                new (&str_tok) std::string(other.str_tok);
            } break;

            case JSONParser::TokenType::PANCTUATION:
            {
                panc_tok = other.panc_tok;
            } break;

            case JSONParser::TokenType::INT:
            {
                int_tok = other.int_tok;
            } break;

            case JSONParser::TokenType::DOUBLE:
            {
                double_tok = other.double_tok;
            } break;
        }
    }

    JSONParser::Token::~Token()
    {
        switch(type)
        {
            case JSONParser::TokenType::NULL_TYPE:
            case JSONParser::TokenType::INT:
            case JSONParser::TokenType::DOUBLE:
            case JSONParser::TokenType::PANCTUATION:
            case JSONParser::TokenType::NUM_TOKEN_TYPES:
            {
            } break;
    
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
    
    void JSONParser::Lex(std::string json_str)
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

            if (std::isspace(json_str[0]))
            {
                while (std::isspace(json_str[0]))
                {
                    json_str.erase(json_str.begin());
                }

                continue;
            }
    
            if (tokens.back().type == TokenType::PANCTUATION &&
                tokens.back().panc_tok == '"')
            {
                LexString(json_str);
                json_str.erase(0,
                                tokens.back().str_tok.size());
                continue;
            }
            
            if (std::isdigit(json_str[0]))
            {
                u8 num_digits = LexNumber(json_str);
                json_str.erase(0, num_digits);

                continue;
            }       
        }
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

            case ':':
            {
                tokens.push_back(Token(':'));
            } break;
            
            case ',':
            {
                tokens.push_back(Token(','));
            } break;
        }
    }
    
    void JSONParser::LexString(const std::string& json_str)
    {
        std::string str;

        auto iter = json_str.begin();
        while (*iter != '"')
        {
            ++iter;
        }
            
        str.insert(str.begin(), json_str.begin(), iter);
        tokens.push_back(Token(str));
    }
    
    u8 JSONParser::LexNumber(const std::string& json_str)
    {
        b8 is_float = 0;
        std::string num;

        u64 index = 0;
        while (1)
        {
            if (json_str[index] == '.')
            {
                is_float = 1;
            }

            num += json_str[index];
            
            ++index;
            if (!std::isdigit(json_str[index]))
            {
                break;
            }
        }

        if (is_float)
        {
            tokens.push_back(Token(std::stod(num)));
        }
        else 
        {
            tokens.push_back(Token(std::stoi(num)));
        }

        return num.size();
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



















