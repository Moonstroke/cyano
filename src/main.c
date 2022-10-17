#include <stdio.h> /* for fprintf, stderr, fputs */
#include <stdlib.h> /* for EXIT_*, free */
#include <string.h> /* for strlen, strnlen, memcmp */

#include "app.h"
#include "grid.h"
#include "gridwindow.h"
#include "file_io.h"
#include "stringutils.h"



const char WINDOW_TITLE[] = "Cyano - Game of Life";


int main(int argc, char **argv) {

	unsigned int grid_width = DEFAULT_GRID_WIDTH;
	unsigned int grid_height = DEFAULT_GRID_HEIGHT;
	unsigned int cell_pixels = DEFAULT_CELLS_PIXELS;
	unsigned int update_rate = DEFAULT_UPDATE_RATE;
	unsigned int border_width = DEFAULT_BORDER_WIDTH;
	bool wrap = false;
	const char *game_rule = DEFAULT_GRID_RULE;
	const char *in_file = NULL;
	const char *out_file = NULL;
	enum grid_format format = GRID_FORMAT_UNKNOWN;

	int rc = parse_cmdline(argc, argv, &grid_width, &grid_height, &wrap,
	                       &game_rule, &cell_pixels, &border_width,
	                       &update_rate, &in_file, &out_file, &format);
	if (rc < 0) {
		return EXIT_FAILURE;
	} else if (rc > 0) {
		/* Found option which mandates termination: --help, --usage */
		return EXIT_SUCCESS;
	}

	if (init_app() < 0) {
		return EXIT_FAILURE;
	}

	struct grid g;
	char *repr = NULL;
	if (in_file != NULL) {
		repr = read_file(in_file);
		if (repr == NULL) {
			fprintf(stderr, "Could not read from file \"%s\"\n", in_file);
			return EXIT_FAILURE;
		}
		/* Override format on recognized file extension */
		if (format == GRID_FORMAT_UNKNOWN) {
			if (endswith(in_file, ".rle")) {
				format = GRID_FORMAT_RLE;
			} else if (endswith(in_file, ".cells")) {
				format = GRID_FORMAT_PLAIN;
			}
		}
		rc = load_grid(&g, repr, format, wrap);
		if (rc < 0) {
			fputs("Failure in creation of the game grid\n", stderr);
			return EXIT_FAILURE;
		}
	} else if (init_grid(&g, grid_width, grid_height, wrap) < 0) {
		fputs("Failure in creation of the game grid\n", stderr);
		return EXIT_FAILURE;
	}
	g.rule = game_rule;

	struct grid_window gw;
	if (init_grid_window(&gw, &g, cell_pixels, border_width,
	                     WINDOW_TITLE) < 0) {
		fprintf(stderr, "Failure in creation of the game window: %s\n",
		        gw.error_msg);
		return EXIT_FAILURE;
	}

	run_app(&gw, update_rate, repr, format, out_file);

	free(repr);
	free_grid(&g);
	free_grid_window(&gw);
	terminate_app();

	return EXIT_SUCCESS;
}
