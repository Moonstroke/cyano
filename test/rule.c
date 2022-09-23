#include "grid.h"

#include <CUTE/cute.h>

#include "rules.h" /* for compile_rulestring */



/* The grid to run tests on */
static struct grid grid;

/* The instance of test case */
CUTE_TestCase *case_rule;



static void setUp(void) {
	memset(grid.rule, 0, sizeof grid.rule);
}


void test_empty_rule(void) {
	const char rule[] = "B/S";
	char expected[sizeof grid.rule] = {0};

	fprintf(stderr, "Testing compilation of rule %s\n", rule);
	int rc = compile_rulestring(rule, grid.rule);
	CUTE_assertEquals(rc, 0);
	CUTE_assertEquals(memcmp(grid.rule, expected, sizeof grid.rule), 0);
}

void test_zero_rule(void) {
	const char rule[] = "B0/S0";
	char expected[sizeof grid.rule] = {0};
	SET_BIT(expected, 0, 1);
	SET_BIT(expected, 16, 1);

	fprintf(stderr, "Testing compilation of rule %s\n", rule);
	int rc = compile_rulestring(rule, grid.rule);
	CUTE_assertEquals(rc, 0);
	CUTE_assertEquals(memcmp(grid.rule, expected, sizeof grid.rule), 0);
}

void test_zero_one_rule(void) {
	const char rule[] = "B01/S01";
	char expected[sizeof grid.rule] = {0};
	SET_BIT(expected, 0, 1);
	SET_BIT(expected, 16, 1);
	SET_BIT(expected, 1, 1);
	SET_BIT(expected, 2, 1);
	SET_BIT(expected, 4, 1);
	SET_BIT(expected, 8, 1);
	SET_BIT(expected, 32, 1);
	SET_BIT(expected, 64, 1);
	SET_BIT(expected, 128, 1);
	SET_BIT(expected, 256, 1);
	SET_BIT(expected, 17, 1);
	SET_BIT(expected, 18, 1);
	SET_BIT(expected, 20, 1);
	SET_BIT(expected, 24, 1);
	SET_BIT(expected, 48, 1);
	SET_BIT(expected, 80, 1);
	SET_BIT(expected, 144, 1);
	SET_BIT(expected, 272, 1);

	fprintf(stderr, "Testing compilation of rule %s\n", rule);
	int rc = compile_rulestring(rule, grid.rule);
	CUTE_assertEquals(rc, 0);
	CUTE_assertEquals(memcmp(grid.rule, expected, sizeof grid.rule), 0);
}

void test_default_rule(void) {
	const char rule[] = "B3/S23";
	char expected[sizeof grid.rule] = {0};
	/*          S2          */
	SET_BIT(expected,  19, 1);
	SET_BIT(expected,  21, 1);
	SET_BIT(expected,  22, 1);
	SET_BIT(expected,  25, 1);
	SET_BIT(expected,  26, 1);
	SET_BIT(expected,  28, 1);
	SET_BIT(expected,  49, 1);
	SET_BIT(expected,  50, 1);
	SET_BIT(expected,  52, 1);
	SET_BIT(expected,  56, 1);
	SET_BIT(expected, 81, 1);
	SET_BIT(expected, 82, 1);
	SET_BIT(expected, 84, 1);
	SET_BIT(expected, 88, 1);
	SET_BIT(expected, 112, 1);
	SET_BIT(expected, 145, 1);
	SET_BIT(expected, 146, 1);
	SET_BIT(expected, 148, 1);
	SET_BIT(expected, 152, 1);
	SET_BIT(expected, 176, 1);
	SET_BIT(expected, 208, 1);
	SET_BIT(expected, 273, 1);
	SET_BIT(expected, 274, 1);
	SET_BIT(expected, 276, 1);
	SET_BIT(expected, 280, 1);
	SET_BIT(expected, 304, 1);
	SET_BIT(expected, 336, 1);
	SET_BIT(expected, 400, 1);
	/*           B3                       S3           */
	SET_BIT(expected,   7, 1); SET_BIT(expected,  23, 1);
	SET_BIT(expected,  11, 1); SET_BIT(expected,  27, 1);
	SET_BIT(expected,  13, 1); SET_BIT(expected,  29, 1);
	SET_BIT(expected,  14, 1); SET_BIT(expected,  30, 1);
	SET_BIT(expected,  35, 1); SET_BIT(expected,  51, 1);
	SET_BIT(expected,  37, 1); SET_BIT(expected,  53, 1);
	SET_BIT(expected,  38, 1); SET_BIT(expected,  54, 1);
	SET_BIT(expected,  41, 1); SET_BIT(expected,  57, 1);
	SET_BIT(expected,  42, 1); SET_BIT(expected,  58, 1);
	SET_BIT(expected,  44, 1); SET_BIT(expected,  60, 1);
	SET_BIT(expected,  67, 1); SET_BIT(expected,  83, 1);
	SET_BIT(expected,  69, 1); SET_BIT(expected,  85, 1);
	SET_BIT(expected,  70, 1); SET_BIT(expected,  86, 1);
	SET_BIT(expected,  73, 1); SET_BIT(expected,  89, 1);
	SET_BIT(expected,  74, 1); SET_BIT(expected,  90, 1);
	SET_BIT(expected,  76, 1); SET_BIT(expected,  92, 1);
	SET_BIT(expected,  97, 1); SET_BIT(expected, 113, 1);
	SET_BIT(expected,  98, 1); SET_BIT(expected, 114, 1);
	SET_BIT(expected, 100, 1); SET_BIT(expected, 116, 1);
	SET_BIT(expected, 104, 1); SET_BIT(expected, 120, 1);
	SET_BIT(expected, 131, 1); SET_BIT(expected, 147, 1);
	SET_BIT(expected, 133, 1); SET_BIT(expected, 149, 1);
	SET_BIT(expected, 134, 1); SET_BIT(expected, 150, 1);
	SET_BIT(expected, 137, 1); SET_BIT(expected, 153, 1);
	SET_BIT(expected, 138, 1); SET_BIT(expected, 154, 1);
	SET_BIT(expected, 140, 1); SET_BIT(expected, 156, 1);
	SET_BIT(expected, 161, 1); SET_BIT(expected, 177, 1);
	SET_BIT(expected, 162, 1); SET_BIT(expected, 178, 1);
	SET_BIT(expected, 164, 1); SET_BIT(expected, 180, 1);
	SET_BIT(expected, 168, 1); SET_BIT(expected, 184, 1);
	SET_BIT(expected, 193, 1); SET_BIT(expected, 209, 1);
	SET_BIT(expected, 194, 1); SET_BIT(expected, 210, 1);
	SET_BIT(expected, 196, 1); SET_BIT(expected, 212, 1);
	SET_BIT(expected, 200, 1); SET_BIT(expected, 216, 1);
	SET_BIT(expected, 224, 1); SET_BIT(expected, 240, 1);
	SET_BIT(expected, 259, 1); SET_BIT(expected, 275, 1);
	SET_BIT(expected, 261, 1); SET_BIT(expected, 277, 1);
	SET_BIT(expected, 262, 1); SET_BIT(expected, 278, 1);
	SET_BIT(expected, 265, 1); SET_BIT(expected, 281, 1);
	SET_BIT(expected, 266, 1); SET_BIT(expected, 282, 1);
	SET_BIT(expected, 268, 1); SET_BIT(expected, 284, 1);
	SET_BIT(expected, 289, 1); SET_BIT(expected, 305, 1);
	SET_BIT(expected, 290, 1); SET_BIT(expected, 306, 1);
	SET_BIT(expected, 292, 1); SET_BIT(expected, 308, 1);
	SET_BIT(expected, 296, 1); SET_BIT(expected, 312, 1);
	SET_BIT(expected, 321, 1); SET_BIT(expected, 337, 1);
	SET_BIT(expected, 322, 1); SET_BIT(expected, 338, 1);
	SET_BIT(expected, 324, 1); SET_BIT(expected, 340, 1);
	SET_BIT(expected, 328, 1); SET_BIT(expected, 344, 1);
	SET_BIT(expected, 352, 1); SET_BIT(expected, 368, 1);
	SET_BIT(expected, 385, 1); SET_BIT(expected, 401, 1);
	SET_BIT(expected, 386, 1); SET_BIT(expected, 402, 1);
	SET_BIT(expected, 388, 1); SET_BIT(expected, 404, 1);
	SET_BIT(expected, 392, 1); SET_BIT(expected, 408, 1);
	SET_BIT(expected, 416, 1); SET_BIT(expected, 432, 1);
	SET_BIT(expected, 448, 1); SET_BIT(expected, 464, 1);

	fprintf(stderr, "Testing compilation of rule %s\n", rule);
	int rc = compile_rulestring(rule, grid.rule);
	CUTE_assertEquals(rc, 0);
	CUTE_assertEquals(memcmp(grid.rule, expected, sizeof grid.rule), 0);
}

void test_full_rule(void) {
	const char rule[] = "B012345678/S012345678";
	char expected[sizeof grid.rule];
	memset(expected, -1, sizeof grid.rule);

	fprintf(stderr, "Testing compilation of rule %s\n", rule);
	int rc = compile_rulestring(rule, grid.rule);
	CUTE_assertEquals(rc, 0);
	CUTE_assertEquals(memcmp(grid.rule, expected, sizeof grid.rule), 0);
}


void build_case_rule(void) {
	case_rule = CUTE_newTestCase("Tests for the rule compilation code", 5);
	CUTE_setCaseBefore(case_rule, setUp);
	CUTE_addCaseTest(case_rule, CUTE_makeTest(test_empty_rule));
	CUTE_addCaseTest(case_rule, CUTE_makeTest(test_zero_rule));
	CUTE_addCaseTest(case_rule, CUTE_makeTest(test_zero_one_rule));
	CUTE_addCaseTest(case_rule, CUTE_makeTest(test_default_rule));
	CUTE_addCaseTest(case_rule, CUTE_makeTest(test_full_rule));
}
