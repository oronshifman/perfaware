/* ------------------------------------------*/ 
/* Filename: JSONParser.h                    */
/* Date:     24.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#ifndef __JSONPARSER_H__
#define __JSONPARSER_H__

#include <fstream>
#include <vector>

#include "JSONObject.h"
#include "my_int.h"

namespace JSORON
{
    class JSONParser 
    {
#ifndef NDEBUG
    public:
#endif /* NDEBUG */    
        enum class TokenType
        {
            NULL_TYPE,
    
            STR,
            PANCTUATION,
            INT,
            DOUBLE,
    
            NUM_TOKEN_TYPES
        };
    
        class Token
        {
        public:
            TokenType type;
    
            union
            {
                std::string str_tok;
                char panc_tok;
                s32 int_tok;
                f64 double_tok;
            };
    
            Token() : type(TokenType::NULL_TYPE) {}
            Token(const Token& other);

            Token(const std::string str_tok) : type(TokenType::STR), str_tok(str_tok) {}
            Token(const char panc_tok) : type(TokenType::PANCTUATION), panc_tok(panc_tok) {}
            Token(const s32 int_tok) : type(TokenType::INT), int_tok(int_tok) {}
            Token(const f64 double_tok) : type(TokenType::DOUBLE), double_tok(double_tok) {}
    
            ~Token();
        };
    
    public:
        JSONObject Parse(const std::string& json_str);
        JSONObject Parse(const std::ifstream& json_file); 
#ifdef NDEBUG
    private:
#endif /* NDEBUG */
        static const JSONObject bad_obj;
    
        void Lex(std::string json_str);

        void LexPanctioation(const char panc);
        void LexString(const std::string& json_str);
        u8 LexNumber(const std::string& json_str);

        /**
         * @brief determines if a char is {,},[,] or :
         */
        b8 IsPanctioation(const char panc);

        typedef std::vector<Token> TokenList;
        TokenList tokens;
    };
}

#endif /* JSONPARSER_H */

