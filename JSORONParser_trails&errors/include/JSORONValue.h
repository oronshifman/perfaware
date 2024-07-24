/* ------------------------------------------*/ 
/* Filename: JSORONValue.h                   */
/* Date:     23.07.2024                      */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#ifndef __JSORON_VALUE_H__
#define __JSORON_VALUE_H__

#include <string>
#include <vector>

#include "JSORONObject.h"

namespace JSORONType
{
    enum JSORONType
    {
        ARRAY, // NOTE: might be a different class altogether
        JSORON_OBJECT,
        STRING,
        FLOAT,
        INT,

        NUM_JSORONTypes    
    };
} // JSORONType

struct JSORONValue 
{
    JSORONType::JSORONType type;

    union
    {
        std::string str_value;   
        JSORONObject json_value;
        u32 int_value;
        f64 float_value;
    };
    
};

#endif /* JSORON_VALUE_H */
