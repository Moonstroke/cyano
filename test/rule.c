#include "grid.h"

#include <CUTE/cute.h>



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
	int rc = compile_grid_rule(&grid, rule);
	CUTE_assertEquals(rc, 0);
	CUTE_assertEquals(memcmp(grid.rule, expected, sizeof grid.rule), 0);
}

void test_zero_rule(void) {
	const char rule[] = "B0/S0";
	char expected[sizeof grid.rule] = {0};
	SET_BIT(expected, 0, 1);
	SET_BIT(expected, 16, 1);

	fprintf(stderr, "Testing compilation of rule %s\n", rule);
	int rc = compile_grid_rule(&grid, rule);
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
	int rc = compile_grid_rule(&grid, rule);
	CUTE_assertEquals(rc, 0);
	CUTE_assertEquals(memcmp(grid.rule, expected, sizeof grid.rule), 0);
}

void test_default_rule(void) {
	const char rule[] = "B3/S23";
	char expected[sizeof grid.rule] = {0};
	// TODO work out expected

	fprintf(stderr, "Testing compilation of rule %s\n", rule);
	int rc = compile_grid_rule(&grid, rule);
	CUTE_assertEquals(rc, 0);
	CUTE_assertEquals(memcmp(grid.rule, expected, sizeof grid.rule), 0);
}

void test_full_rule(void) {
	const char rule[] = "B012345678/S012345678";
	char expected[sizeof grid.rule];
	memset(expected, -1, sizeof grid.rule);

	fprintf(stderr, "Testing compilation of rule %s\n", rule);
	int rc = compile_grid_rule(&grid, rule);
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
