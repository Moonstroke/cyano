#include "clop.h"

#include <clog.h> /* for logging macros */
#include <CUTE/cute.h>

#include "board.h" /* for DEFAULT_BOARD_RULES */



static unsigned int width = 0, height = 0, cell_size = 0, update_rate = 0,
                    border_size = 0;
static bool use_vsync = false, wraps = false;
static const char *rules = DEFAULT_BOARD_RULES;

/* The instance of test case */
CUTE_TestCase *case_clop;


static void setUp(void) {
	setvars(&width, &height, &cell_size, &border_size, &update_rate, &use_vsync,
	        &wraps, &rules);
}


void test_clop() {
	const unsigned int argc = 0;
	char *argv[] = {"argv[0]", NULL};
	int status;
	notice("Test clop's getvals()");
	status = getvals(argc, argv, OPTSTRING, LONGOPTS);
	CUTE_assertEquals(status, 0);
}


void build_case_clop(void) {
	case_clop = CUTE_newTestCase("Test for clop", 1);
	CUTE_setCaseAfter(case_clop, setUp);
	CUTE_addCaseTest(case_clop, CUTE_makeTest(test_clop));
}
