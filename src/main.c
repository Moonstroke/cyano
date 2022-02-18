#include <stdlib.h> /* for EXIT_* */

#include "app.h"
#include "board.h"
#include "boardwindow.h"



int main(int argc, char **argv) {

	unsigned int board_width = DEFAULT_BOARD_WIDTH,
	             board_height = DEFAULT_BOARD_HEIGHT,
	             cell_pixels = DEFAULT_CELLS_PIXELS,
	             update_rate = DEFAULT_UPDATE_RATE,
	             border_width = DEFAULT_BORDER_WIDTH;
	bool use_vsync = false, wrap = false;
	const char *game_rules = DEFAULT_BOARD_RULES;
	const char *file = NULL;

	if (parseCommandLineArgs(argc, argv, &board_width, &board_height, &wrap,
	                         &game_rules, &cell_pixels, &border_width,
	                         &update_rate, &use_vsync, &file) < 0) {
		return EXIT_FAILURE;
	}

	if (initApp() < 0) {
		return EXIT_FAILURE;
	}

	struct board b;
	if (initBoard(&b, board_width, board_height, wrap) < 0) {
		fputs("Failure in creation of the game board\n", stderr);
		return EXIT_FAILURE;
	}
	b.rules = game_rules;

	struct boardwindow bw;
	if (initBoardWindow(&bw, &b, cell_pixels, border_width, "SDL Game of Life",
	                    use_vsync) < 0) {
		fprintf(stderr, "Failure in creation of the game window: %s\n",
		        bw.error_msg);
		return EXIT_FAILURE;
	}

	runApp(&bw, update_rate, use_vsync);

	freeBoard(&b);
	freeBoardWindow(&bw);
	terminateApp();

	return EXIT_SUCCESS;
}
