/* SPDX-License-Identifier: CECILL-2.1 */
#include "grid.h"

#include <CUTE/cute.h>
#include <stdio.h> /* for fprintf, stderr, fputs */
#include <stdlib.h> /* for free */
#include <string.h> /* for memcpy */



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
	unsigned int col;
	fputs("-- Test for the status of a blinker after one generation\n",
	      stderr);
	fputs("Create vertical blinker from (0, 1) to (2, 1)\n", stderr);
	for(unsigned int row = 0; row < 3; ++row) {
		toggle_cell(&grid, row, 1);
	}

	fputs("Next generation\n", stderr);
	update_grid(&grid);

	fputs("Looking for horizontal blinker from (1, 0) to (1, 2)\n", stderr);
	for(col = 0; col < 3; ++col) {
		CUTE_assertEquals(get_grid_cell(&grid, 1, col), ALIVE);
	}
	fputs("Looking for empty rows 0 and 2\n", stderr);
	for (col = 0; col < 3; ++col) {
		CUTE_assertEquals(get_grid_cell(&grid, 0, col), DEAD);
		CUTE_assertEquals(get_grid_cell(&grid, 2, col), DEAD);
	}
	fputs("OK\n", stderr);
}

void test_blinker_after_two_gens(void) {
	unsigned int col;
	fputs("-- Test for the status of a blinker after two generations\n",
	      stderr);
	fputs("Create vertical blinker from (1, 0) to (1, 2)\n", stderr);
	for(col = 0; col < 3; ++col) {
		toggle_cell(&grid, 1, col);
	}

	fputs("Next generation\n", stderr);
	update_grid(&grid);
	fputs("Next generation\n", stderr);
	update_grid(&grid);

	fputs("Looking for vertical blinker from (0, 1) to (2, 1)\n", stderr);
	for(col = 0; col < 3; ++col) {
		CUTE_assertEquals(get_grid_cell(&grid, 1, col), ALIVE);
	}
	fputs("Looking for empty columns 0 and 2\n", stderr);
	for (col = 0; col < 3; ++col) {
		CUTE_assertEquals(get_grid_cell(&grid, 0, col), DEAD);
		CUTE_assertEquals(get_grid_cell(&grid, 2, col), DEAD);
	}
	fputs("OK\n", stderr);
}

void test_glider_rle_repr(void) {
	const char expected[] = "x = 3, y = 3, rule = B3/S23\nbo$2bo$3o!";
	fputs("-- Test for the RLE representation of a blinker\n", stderr);
	fputs("Create glider\n", stderr);
	toggle_cell(&grid, 0, 1);
	toggle_cell(&grid, 1, 2);
	toggle_cell(&grid, 2, 0);
	toggle_cell(&grid, 2, 1);
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
