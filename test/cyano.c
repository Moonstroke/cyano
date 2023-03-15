/* SPDX-License-Identifier: CECILL-2.1 */
#include <CUTE/cute.h>
#include <stdlib.h> /* for EXIT_SUCCESS */



extern CUTE_TestCase *case_grid;
extern void build_case_grid(void);
extern CUTE_TestCase *case_bits;
extern void build_case_bits(void);

int main(void) {
	const CUTE_RunResults **results;

	build_case_grid();
	build_case_bits();

	CUTE_prepareTestSuite(2, case_grid, case_bits);

	results = CUTE_runTestSuite();

	CUTE_printResults(2, results);

	return EXIT_SUCCESS;
}
