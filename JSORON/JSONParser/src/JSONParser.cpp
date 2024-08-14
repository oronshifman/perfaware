/* ------------------------------------------*/ 
/* Filename: JSONParser.cpp                  */
/* Date:     24.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include <cctype>
#include <fstream>
#include <string>
#include <unordered_set>

#include "JSONParser.h"
#include "JSONObject.h"
#include "my_int.h"

namespace JSORON
{
    JSONParser::Token::Token(const Token& other) : type(other.type)
    {
        AssignTokByType(*this, other, other.type);
    }

    JSONParser::Token& JSONParser::Token::operator=(const JSONParser::Token& other)
    {
        if (this == &other)
        {
            return *this;
        }

        AssignTokByType(*this, other, other.type);
        return *this;
    }        

    void JSONParser::Token::AssignTokByType(Token& dest, const Token& src, TokenType type)
    {
        switch (type)
        {
            case JSONParser::TokenType::STR:
            {
                new (&dest.str_tok) std::string(src.str_tok);
            } break;

            case JSONParser::TokenType::PANCTUATION:
            {
                dest.panc_tok = src.panc_tok;
            } break;

            case JSONParser::TokenType::INT:
            {
                dest.int_tok = src.int_tok;
            } break;

            case JSONParser::TokenType::DOUBLE:
            {
                dest.double_tok = src.double_tok;
            } break;

            case JSONParser::TokenType::NULL_TYPE:
            {
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
            {
            } break;
    
            case JSONParser::TokenType::STR:
            {
                str_tok.~basic_string();
            } break;
        }
    }
    
    const JSONObject JSONParser::bad_obj = JSONObject();
    
    JSONObject JSONParser::Parse(const std::ifstream& json_file)
    {
        // TODO(7.8.24): impl
        return bad_obj;
    }
    
    JSONObject JSONParser::Parse(const std::string& json_str)
    {
        Lex(json_str);
        return *(_Parse().json_val);
    }

    JSONObject::JSONValue JSONParser::_Parse()
    {
        // TODO(10.8.24): should be done, might still need some changes
        Token curr_tok = tokens[0];
        tokens.erase(tokens.begin());

        if (curr_tok.type == TokenType::PANCTUATION)
        {
            if (curr_tok.panc_tok == '{')
            {
                return ParseObj();
            }
            else if (curr_tok.panc_tok == '[')
            {
                return ParseArray();
            }
            else 
            {
                return JSONObject::JSONValue();
            }
        }

        switch (curr_tok.type)
        {
            case TokenType::DOUBLE:
            {
                return JSONObject::JSONValue(curr_tok.double_tok);
            } break;

            case TokenType::INT:
            {
                return JSONObject::JSONValue(curr_tok.int_tok);
            } break;

            case TokenType::STR:
            {
                if (tokens[0].type == TokenType::PANCTUATION &&
                    tokens[0].panc_tok == ':')
                {
                    return JSONObject::JSONValue(JSONObject::ValueType::KEY, curr_tok.str_tok);
                }
                else
                {
                    return JSONObject::JSONValue(curr_tok.str_tok);
                }
            } break;
        }

        return JSONObject::bad_value;
    }
    
    JSONObject::JSONValue JSONParser::ParseObj()
    {
        // TODO(10.8.24): impl 
        JSONObject obj;
        
        Token curr_tok = tokens[0];
        while (IsEndOfObj(curr_tok))
        {
            JSONObject::JSONValue key = _Parse();
            if (key.type == JSONObject::ValueType::NULL_TYPE)
            {
                continue;
            }
            if (key.type == JSONObject::ValueType::KEY)
            {
                JSONObject::JSONValue val = _Parse();
                if (val.type == JSONObject::ValueType::NULL_TYPE ||
                    val.type == JSONObject::ValueType::BAD_TYPE)
                {
                    // SyntaxError(); TODO(14.8.24): work on SyntaxError()
                    break;
                }
                obj.Put(key.str_val, val); 
            }
            curr_tok = tokens[0];
        }

        return JSONObject::JSONValue(obj);
    }

    b8 JSONParser::IsEndOfObj(const Token& tok)
    {
        if (tok.type == TokenType::PANCTUATION &&
            tok.panc_tok == '}')
        {
            return 1;
        }

        return 0;
    }

    JSONObject::JSONValue JSONParser::ParseArray()
    {
        // TODO(10.8.24): impl
    }

    static void SyntaxError(std::string where, char expected, char got)
    {
        fprintf(stderr, "Syntax error: After %s, got: %c, expected: %c", where.c_str(), got, expected);
    }
    
    void JSONParser::Lex(std::string json_str)
    {
        while (!json_str.empty())
        {
            if (std::ispunct(json_str[0]))
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
    
            if (std::isalpha(json_str[0]))
            {
                LexString(json_str);
                json_str.erase(0, tokens.back().str_tok.size());
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
    
            case ':':
            {
                tokens.push_back(Token(':'));
            } break;
            
            case ',':
            {
                tokens.push_back(Token(','));
            } break;
            
            case '"':
            {
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

} // namespace JSORON



















