#include <CUTE/cute.h>
#include <stdlib.h> /* for EXIT_SUCCESS */



extern CUTE_TestCase *case_board;
extern void build_case_board(void);

extern CUTE_TestCase *case_clop;
extern void build_case_clop(void);

int main() {
	CUTE_RunResults **results;

	build_case_board();
	build_case_clop();

	CUTE_prepareTestSuite(2, case_board, case_clop);

	results = CUTE_runTestSuite();

	CUTE_printResults(2, results);

	return EXIT_SUCCESS;
}
