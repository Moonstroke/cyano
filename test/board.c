#include "grid.h"

#include <CUTE/cute.h>
#include <stdlib.h> /* for free */



/* The dimensions of the grid */
static const unsigned int WIDTH = 3;
static const unsigned int HEIGHT = 3;
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
	status = init_grid(&grid, WIDTH, HEIGHT, WRAPS);
	CUTE_assertEquals(status, 0);
	memcpy(grid.rule, RULE, sizeof RULE);
	fprintf(stderr, "Succesfully initialized %s grid %ux%u\n",
	        WRAPS ? "toroidal" : "rectangular", WIDTH, HEIGHT);
}

static void tearDown(void) {
	free_grid(&grid);
}


void test_blinker_after_one_gen(void) {
	unsigned int i;
	fputs("-- Test for the status of a blinker after one generation\n",
	      stderr);
	fputs("Create vertical blinker from (1, 0) to (1, 2)\n", stderr);
	for(i = 0; i < 3; ++i) {
		toggle_cell(&grid, 1, i);
	}

	fputs("Next generation\n", stderr);
	update_grid(&grid);

	fputs("Looking for horizontal blinker from (0, 1) to (2, 1)\n", stderr);
	for(i = 0; i < 3; ++i) {
		CUTE_assertEquals(get_grid_cell(&grid, i, 1), ALIVE);
	}
	fputs("Looking for empty rows 0 and 2\n", stderr);
	for (i = 0; i < 3; ++i) {
		CUTE_assertEquals(get_grid_cell(&grid, i, 0), DEAD);
		CUTE_assertEquals(get_grid_cell(&grid, i, 2), DEAD);
	}
	fputs("OK\n", stderr);
}

void test_blinker_after_two_gens(void) {
	unsigned int i;
	fputs("-- Test for the status of a blinker after two generations\n",
	      stderr);
	fputs("Create vertical blinker from (1, 0) to (1, 2)\n", stderr);
	for(i = 0; i < 3; ++i) {
		toggle_cell(&grid, 1, i);
	}

	fputs("Next generation\n", stderr);
	update_grid(&grid);
	fputs("Next generation\n", stderr);
	update_grid(&grid);

	fputs("Looking for vertical blinker from (0, 1) to (2, 1)\n", stderr);
	for(i = 0; i < 3; ++i) {
		CUTE_assertEquals(get_grid_cell(&grid, 1, i), ALIVE);
	}
	fputs("Looking for empty columns 0 and 2\n", stderr);
	for (i = 0; i < 3; ++i) {
		CUTE_assertEquals(get_grid_cell(&grid, 0, i), DEAD);
		CUTE_assertEquals(get_grid_cell(&grid, 2, i), DEAD);
	}
	fputs("OK\n", stderr);
}

void test_glider_rle_repr(void) {
	const char expected[] = "x = 3, y = 3, rule = B3/S23\nbo$2bo$3o!";
	fputs("-- Test for the RLE representation of a blinker\n", stderr);
	fputs("Create glider\n", stderr);
	toggle_cell(&grid, 1, 0);
	toggle_cell(&grid, 2, 1);
	toggle_cell(&grid, 0, 2);
	toggle_cell(&grid, 1, 2);
	toggle_cell(&grid, 2, 2);
	fprintf(stderr, "RLE repr expected: \"%s\"\n", expected);
	char *repr = get_grid_repr(&grid, GRID_FORMAT_RLE);
	fprintf(stderr, "RLE repr got:      \"%s\"\n", repr);
	int cmp = strcmp(repr, expected);
	free(repr);
	CUTE_assertEquals(cmp, 0);
}

void build_case_grid(void) {
	case_grid = CUTE_newTestCase("Tests for the grid structure", 3);
	CUTE_setCaseBefore(case_grid, setUp);
	CUTE_setCaseAfter(case_grid, tearDown);
	CUTE_addCaseTest(case_grid, CUTE_makeTest(test_blinker_after_one_gen));
	CUTE_addCaseTest(case_grid, CUTE_makeTest(test_blinker_after_two_gens));
	CUTE_addCaseTest(case_grid, CUTE_makeTest(test_glider_rle_repr));
}
