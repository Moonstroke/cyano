/* SPDX-License-Identifier: CECILL-2.1 */
#include <stdio.h> /* for fprintf, stderr, fputs */
#include <stdlib.h> /* for EXIT_*, free */
#include <string.h> /* for strlen, strnlen, memcmp */

#include "app.h"
#include "grid.h"
#include "gridwindow.h"
#include "file_io.h"
#include "stringutils.h"



static inline enum grid_format _guess_format_from_ext(const char *fpath) {
	if (endswith(fpath, ".rle")) {
		return GRID_FORMAT_RLE;
	}
	if (endswith(fpath, ".cells")) {
		return GRID_FORMAT_PLAIN;
	}
	return GRID_FORMAT_UNKNOWN;
}
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

	struct grid grid;
	char *repr = NULL;
	if (in_file != NULL) {
		repr = read_file(in_file);
		if (repr == NULL) {
			fprintf(stderr, "Could not read from file \"%s\"\n", in_file);
			return EXIT_FAILURE;
		}
		/* Override format on recognized file extension */
		if (format == GRID_FORMAT_UNKNOWN) {
			format = _guess_format_from_ext(in_file);
		}
		rc = load_grid(&grid, repr, format, wrap);
		if (rc < 0) {
			fputs("Failure in creation of the game grid\n", stderr);
			return EXIT_FAILURE;
		}
	} else if (init_grid(&grid, grid_width, grid_height, wrap) < 0) {
		fputs("Failure in creation of the game grid\n", stderr);
		return EXIT_FAILURE;
	}
	memcpy(grid.rule, game_rule, sizeof grid.rule);

	struct grid_window grid_win;
	if (init_grid_window(&grid_win, &grid, cell_pixels, border_width,
	                     WINDOW_TITLE) < 0) {
		fprintf(stderr, "Failure in creation of the game window: %s\n",
		        grid_win.error_msg);
		return EXIT_FAILURE;
	}

	enum grid_format out_fmt;
	if (out_file != NULL) {
		out_fmt = _guess_format_from_ext(out_file);
	} else {
		out_fmt = GRID_FORMAT_UNKNOWN;
	}
	run_app(&grid_win, update_rate, repr, format, out_file, out_fmt);

	free(repr);
	free_grid(&grid);
	free_grid_window(&grid_win);
	terminate_app();

	return EXIT_SUCCESS;
}
