#include <CUTE/cute.h>
#include <stdlib.h> /* for EXIT_SUCCESS */



extern CUTE_TestCase *case_board;
extern void build_case_board(void);

int main() {
	CUTE_RunResults **results;

	build_case_board();

	CUTE_prepareTestSuite(1, case_board);

	results = CUTE_runTestSuite();

	CUTE_printResults(1, results);

	return EXIT_SUCCESS;
}
