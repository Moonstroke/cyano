#include <stdlib.h> /* for EXIT_*, free */

#include "app.h"
#include "grid.h"
#include "gridwindow.h"
#include "file_io.h"



int main(int argc, char **argv) {

	unsigned int grid_width = DEFAULT_GRID_WIDTH;
	unsigned int grid_height = DEFAULT_GRID_HEIGHT;
	unsigned int cell_pixels = DEFAULT_CELLS_PIXELS;
	unsigned int update_rate = DEFAULT_UPDATE_RATE;
	unsigned int border_width = DEFAULT_BORDER_WIDTH;
	bool use_vsync = false;
	bool wrap = false;
	const char *game_rule = DEFAULT_GRID_RULE;
	const char *in_file = NULL;
	const char *out_file = NULL;

	int rc = parseCommandLineArgs(argc, argv, &grid_width, &grid_height, &wrap,
	                              &game_rule, &cell_pixels, &border_width,
	                              &update_rate, &use_vsync, &in_file,
	                              &out_file);
	if (rc < 0) {
		return EXIT_FAILURE;
	} else if (rc > 0) {
		/* Found option which mandates termination: --help, --usage */
		return EXIT_SUCCESS;
	}

	if (initApp() < 0) {
		return EXIT_FAILURE;
	}

	struct grid g;
	char *repr = NULL;
	if (in_file != NULL) {
		repr = readFile(in_file);
		if (repr == NULL) {
			fprintf(stderr, "Could not read from file \"%s\"\n", in_file);
			return EXIT_FAILURE;
		}
		int rc = loadGrid(&g, repr, wrap);
		if (rc < 0) {
			fputs("Failure in creation of the game grid\n", stderr);
			return EXIT_FAILURE;
		}
	} else if (initGrid(&g, grid_width, grid_height, wrap) < 0) {
		fputs("Failure in creation of the game grid\n", stderr);
		return EXIT_FAILURE;
	}
	g.rule = game_rule;

	struct gridwindow gw;
	if (initGridWindow(&gw, &g, cell_pixels, border_width, "SDL Game of Life",
	                    use_vsync) < 0) {
		fprintf(stderr, "Failure in creation of the game window: %s\n",
		        gw.error_msg);
		return EXIT_FAILURE;
	}

	runApp(&gw, update_rate, use_vsync, repr, out_file);

	free(repr);
	freeGrid(&g);
	freeGridWindow(&gw);
	terminateApp();

	return EXIT_SUCCESS;
}
