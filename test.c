#include "log.h"

int test_board(void);
int test_clop(int argc, const char *const argv[]);

int main(const int argc, const char *const argv[]) {
	int status;
	info("Test suite for command-line options\n");
	status = test_clop(argc, argv);
	if(status != 0) {
		error("The tests have failed.");
		return status;
	}

	info("\n\n");
	info("Test suite for the Board structure\n");
	status = test_board();
	if(status != 0) {
		error("The test have failed.");
		return status;
	}
	return 0;
}
