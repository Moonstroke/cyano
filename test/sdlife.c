#include <CUTE/cute.h>
#include <stdlib.h> /* for EXIT_SUCCESS */



extern CUTE_TestCase *case_grid;
extern void build_case_grid(void);
extern void tests_bits(void);
extern CUTE_TestCase *case_rule;
extern void build_case_rule(void);

int main() {
	CUTE_RunResults **results;

	build_case_grid();
	build_case_rule();

	CUTE_prepareTestSuite(2, case_grid, case_rule);

	results = CUTE_runTestSuite();

	CUTE_printResults(2, results);

	tests_bits();

	return EXIT_SUCCESS;
}
