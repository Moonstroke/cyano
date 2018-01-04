#include "board.h"

#include "log.h"



int test_board(void) {

	const unsigned int w = 3, h = 3;
	const bool W = false;
	const char *const R = DEFAULT_BOARD_RULES;
	unsigned int i, j;

	info("Init %s board %ux%u", W ? "toroidal" : "rectangular", w, h);
	Board b;
	initBoard(&b, w, h, W);
	setRules(&b, R);

	info("created vertical blinker from (1, 0) to (1, 2)");
	for(j = 0; j < 3; ++j)
		toggleCell(&b, 1, j);

	info("Next generation");
	updateBoard(&b);

	info("Looking for horizontal blinker from (0, 1) to (2, 1)");
	for(i = 0; i < 3 && *(b.getCell(&b, i, 1)); ++i) {
		verbose("Found live cell at (%u, 1)", i);
	}
	if(i == 3)
		info("OK");
	else {
		freeBoard(&b);
		return 1;
	}

	info("Next generation");
	updateBoard(&b);

	info("Looking for horizontal blinker from (0, 1) to (2, 1)");
	for(j = 0; j < 3 && *(b.getCell(&b, 1, j)); ++j) {
		verbose("Found live cell at (1, %u)", j);
	}
	if(j == 3)
		info("OK");
	else {
		freeBoard(&b);
		return 1;
	}

	freeBoard(&b);
	return 0;
}
