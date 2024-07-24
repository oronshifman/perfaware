#include <stdio.h>

#include "generic_test.h"

void TestAll(void)
{
	if (g_test_counter == g_succeeded_test_counter)
	{
		printf(DEFAULT "\nAll %lu tests " BOLDGREEN "SUCCESS\n" DEFAULT, g_test_counter);
	}
	else
	{
		printf(DEFAULT "\n%lu out of %lu tests " BOLDRED "FAIL\n" DEFAULT, 
			   g_test_counter - g_succeeded_test_counter, g_test_counter);
	}
}