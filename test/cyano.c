/* SPDX-License-Identifier: CECILL-2.1 */
#include <CUTE/cute.h>
#include <stdlib.h> /* for EXIT_SUCCESS */



extern CUTE_TestCase *case_grid;
extern void build_case_grid(void);
extern void tests_bits(void);

int main(void) {
	const CUTE_RunResults **results;

	build_case_grid();

	CUTE_prepareTestSuite(1, case_grid);

	results = CUTE_runTestSuite();

	CUTE_printResults(1, results);

	tests_bits();

	return EXIT_SUCCESS;
}
