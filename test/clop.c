#include "clop.h"

#include "board.h"
#include <clog.h>

int test_clop(const int argc, const char *const argv[]) {

	unsigned int w = 0, h = 0, c = 0, r = 0, b = 0;
	bool v = false, W = false;
	const char *R = DEFAULT_BOARD_RULES;
	setvars(&w, &h, &c, &r, &b, &v, &W, &R);

	if(!getvals(argc, argv, OPTSTRING, LONGOPTS))
		error("An error happened");

	debug("\nw = %u, h = %u, c = %u", w, h, c);


	return 0;
}
