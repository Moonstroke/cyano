#include "life.h"

#include "log.h"


int main(void) {

	const unsigned int W = 3, H = 3;
	unsigned int i, j;

	info("Init board %ux%u", W, H);
	Board b;
	initBoard(&b, W, H);

	info("created vertical blinker from (1, 0) to (1, 2)");
	for(j = 0; j < 3; ++j)
		toggleCell(&b, 1, j);

	info("Next generation");
	updateBoard(&b);

	info("Looking for horizontal blinker from (0, 1) to (2, 1)");
	for(i = 0; i < 3 && *getCell(&b, i, 1); ++i) {
		verbose("Found live cell at (%u, 1)", i);
	}
	if(i == 3)
		info("OK");
	else
		return 1;

	info("Next generation");
	updateBoard(&b);

	info("Looking for horizontal blinker from (0, 1) to (2, 1)");
	for(j = 0; j < 3 && *getCell(&b, 1, j); ++j) {
		verbose("Found live cell at (1, %u)", j);
	}
	if(j == 3)
		info("OK");
	else
		return 1;


	return 0;
}
