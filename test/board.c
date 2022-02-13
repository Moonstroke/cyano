#include "board.h"

#include <CUTE/cute.h>



/* The dimensions of the board */
static const unsigned int WIDTH = 3, HEIGHT = 3;
/* Does the board wrap or not? */
static const bool WRAPS = false;
/* The rules of the game to run */
static const char RULES[] = DEFAULT_BOARD_RULES;

/* The board to run tests on */
static struct board board;

/* The instance of test case */
CUTE_TestCase *case_board;



static void setUp(void) {
	int status;
	status = initBoard(&board, WIDTH, HEIGHT, WRAPS);
	CUTE_assertEquals(status, 0);
	setRules(&board, RULES);
	fprintf(stderr, "Succesfully initialized %s board %ux%u\n",
	        WRAPS ? "toroidal" : "rectangular", WIDTH, HEIGHT);
}

static void tearDown(void) {
	freeBoard(&board);
}


void test_blinker_after_one_gen(void) {
	unsigned int i;
	fputs("-- Test for the status of a blinker after one generation\n",
	      stderr);
	fputs("Create vertical blinker from (1, 0) to (1, 2)\n", stderr);
	for(i = 0; i < 3; ++i) {
		toggleCell(&board, 1, i);
	}

	fputs("Next generation\n", stderr);
	updateBoard(&board);

	fputs("Looking for horizontal blinker from (0, 1) to (2, 1)\n", stderr);
	for(i = 0; i < 3; ++i) {
		CUTE_assertEquals(getBoardCell(&board, i, 1), true);
	}
	info("OK");
}

void test_blinker_after_two_gens(void) {
	unsigned int i;
	fputs("-- Test for the status of a blinker after two generations\n",
	      stderr);
	fputs("Create vertical blinker from (1, 0) to (1, 2)\n", stderr);
	for(i = 0; i < 3; ++i) {
		toggleCell(&board, 1, i);
	}

	fputs("Next generation\n", stderr);
	updateBoard(&board);
	fputs("Next generation\n", stderr);
	updateBoard(&board);

	fputs("Looking for vertical blinker from (0, 1) to (2, 1)\n", stderr);
	for(i = 0; i < 3; ++i) {
		CUTE_assertEquals(getBoardCell(&board, 1, i), true);
	}
	info("OK");
}

void build_case_board(void) {
	case_board = CUTE_newTestCase("Tests for the Board structure", 2);
	CUTE_setCaseBefore(case_board, setUp);
	CUTE_setCaseAfter(case_board, tearDown);
	CUTE_addCaseTest(case_board, CUTE_makeTest(test_blinker_after_one_gen));
	CUTE_addCaseTest(case_board, CUTE_makeTest(test_blinker_after_two_gens));
}
