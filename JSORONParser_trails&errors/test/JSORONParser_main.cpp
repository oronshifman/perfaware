/* ------------------------------------------*/ 
/* Filename: JSORONParser_main.cpp           */
/* Date:     22.07.2024              	     */
/* Author:   Oron                            */ 
/* ------------------------------------------*/

#include <iostream>

#include "JSORONObject.h"

int main(int argc, char *argv[])
{
	JSORONObject json;

	// json["pairs"].str_value = "pair1";
	json.PutStr("pairs", "pair1");
	JSORONValue& pair1 = json.Get("pairs");
	
	return 0;
}

