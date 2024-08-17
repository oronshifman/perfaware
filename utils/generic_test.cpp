#include <stdio.h>

#include "generic_test.h"

void Tester::TestAll(void)
{
	if (test_counter == succeeded_test_counter)
	{
		printf(DEFAULT "\nAll %lu tests " BOLDGREEN "SUCCESS\n" DEFAULT, test_counter);
	}
	else
	{
		printf(DEFAULT "\n%lu out of %lu tests " BOLDRED "FAIL\n" DEFAULT, 
			   test_counter - succeeded_test_counter, test_counter);
	}
}
