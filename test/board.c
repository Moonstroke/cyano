#include "grid.h"

#include <CUTE/cute.h>



/* The dimensions of the grid */
static const unsigned int WIDTH = 3, HEIGHT = 3;
/* Does the grid wrap or not? */
static const bool WRAPS = false;
/* The rules of the game to run */
static const char RULE[] = DEFAULT_GRID_RULE;

/* The grid to run tests on */
static struct grid grid;

/* The instance of test case */
CUTE_TestCase *case_grid;



static void setUp(void) {
	int status;
	status = initGrid(&grid, WIDTH, HEIGHT, WRAPS);
	CUTE_assertEquals(status, 0);
	grid.rule = RULE;
	fprintf(stderr, "Succesfully initialized %s grid %ux%u\n",
	        WRAPS ? "toroidal" : "rectangular", WIDTH, HEIGHT);
}

static void tearDown(void) {
	freeGrid(&grid);
}


void test_blinker_after_one_gen(void) {
	unsigned int i;
	fputs("-- Test for the status of a blinker after one generation\n",
	      stderr);
	fputs("Create vertical blinker from (1, 0) to (1, 2)\n", stderr);
	for(i = 0; i < 3; ++i) {
		toggleCell(&grid, 1, i);
	}

	fputs("Next generation\n", stderr);
	updateGrid(&grid);

	fputs("Looking for horizontal blinker from (0, 1) to (2, 1)\n", stderr);
	for(i = 0; i < 3; ++i) {
		CUTE_assertEquals(getGridCell(&grid, i, 1), true);
	}
	info("OK");
}

void test_blinker_after_two_gens(void) {
	unsigned int i;
	fputs("-- Test for the status of a blinker after two generations\n",
	      stderr);
	fputs("Create vertical blinker from (1, 0) to (1, 2)\n", stderr);
	for(i = 0; i < 3; ++i) {
		toggleCell(&grid, 1, i);
	}

	fputs("Next generation\n", stderr);
	updateGrid(&grid);
	fputs("Next generation\n", stderr);
	updateGrid(&grid);

	fputs("Looking for vertical blinker from (0, 1) to (2, 1)\n", stderr);
	for(i = 0; i < 3; ++i) {
		CUTE_assertEquals(getGridCell(&grid, 1, i), true);
	}
	info("OK");
}

void build_case_grid(void) {
	case_grid = CUTE_newTestCase("Tests for the grid structure", 2);
	CUTE_setCaseBefore(case_grid, setUp);
	CUTE_setCaseAfter(case_grid, tearDown);
	CUTE_addCaseTest(case_grid, CUTE_makeTest(test_blinker_after_one_gen));
	CUTE_addCaseTest(case_grid, CUTE_makeTest(test_blinker_after_two_gens));
}
