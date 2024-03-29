/* SPDX-License-Identifier: CECILL-2.1 */
#include "app.h"

#include <limits.h> /* for INT_MIN */
#include <SDL2/SDL.h>
#include <stdio.h> /* for fprintf, stderr, fputs */

#include "grid.h"
#include "gridwindow.h"
#include "file_io.h" /* for write_file */
#include "utils.h" /* for CHECK_NULL */



static const char UI_HELP[] = "Interface usage:\n"
	" - Using the mouse\n"
	"Move the mouse cursor to highlight a cell, and click the left button to "
	"toggle\nits state.\n"
	" - Using the keyboard\n"
	"The following keys or key combinations have an action defined:\n"
	"    Key    Action\n"
	"     C     Clear the grid\n"
	"     H     Display this message\n"
	" Ctrl + Q  Quit the program\n"
	"     R     Reset the grid to the input file\n"
	"     T     Toggle the state of the highlighted cell\n"
	"     W     Write the state of the grid to the output file in plain text\n"
	" Ctrl + W  Quit the program\n"
	"   Space   Toggle pause mode\n"
	"   Enter   When paused, evolve the grid by one generation\n"
	"    Esc    Quit the program\n"
	"Arrow keys Move the highlight by one cell in the key's direction\n";


int init_app(void) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Could not load the SDL: %s\n", SDL_GetError());
		return -__LINE__;
	}

	return 0;
}


static void _handle_mouse_on_cell(struct grid_window *gw, int *last_x,
                                  int *last_y) {
	if (gw->sel_col >= 0 && gw->sel_row >= 0) {
		toggle_cell(gw->grid, gw->sel_row, gw->sel_col);
		*last_x = gw->sel_col;
		*last_y = gw->sel_row;
	}
}

static inline void _reset_grid(struct grid *grid, const char *repr,
                               enum grid_format format, bool *play,
                               bool *loop) {
	if (repr != NULL) {
		if (*play) {
			*play = false;
		}
		bool wrap = grid->wrap;
		free_grid(grid);
		if (load_grid(grid, repr, format, wrap) < 0) {
			fputs("Error while resetting the grid\n", stderr);
			*loop = false;
		}
	}
}

static inline int _output_grid(const struct grid *grid, const char *out_file,
                               enum grid_format out_file_format) {
	char *repr = get_grid_repr(grid, out_file_format);
	CHECK_NULL(repr);
	int rc = write_file(out_file, repr);
	free(repr);
	return rc;
}

static inline void _print_help(void) {
	fwrite(UI_HELP, 1, sizeof UI_HELP, stdout);
}

static void _handle_key_event(const SDL_KeyboardEvent *event,
                              struct grid_window *gw, bool *loop,
                              bool *play, const char *repr,
                              enum grid_format repr_format,
                              const char *out_file,
                              enum grid_format out_file_format) {
	switch (event->keysym.sym) {
		case SDLK_SPACE:
			*play = !*play;
			break;
		case SDLK_RETURN:
			if (!*play) {
				update_grid(gw->grid);
			}
			break;
		case SDLK_UP:
			if (--gw->sel_row < 0) {
				gw->sel_row = gw->grid->wrap ? gw->grid->height - 1 : 0;
			}
			break;
		case SDLK_DOWN:
			if (++gw->sel_row >= (signed) gw->grid->height) {
				gw->sel_row = gw->grid->wrap ? 0 : gw->grid->height - 1;
			}
			break;
		case SDLK_LEFT:
			if (--gw->sel_col < 0) {
				gw->sel_col = gw->grid->wrap ? gw->grid->width - 1 : 0;
			}
			break;
		case SDLK_RIGHT:
			if (++gw->sel_col >= (signed) gw->grid->width) {
				gw->sel_col = gw->grid->wrap ? 0 : gw->grid->width - 1;
			}
			break;
		case SDLK_t:
			toggle_cell(gw->grid, gw->sel_row, gw->sel_col);
			break;
		case SDLK_r:
			_reset_grid(gw->grid, repr, repr_format, play, loop);
			break;
		/* The window can be closed with ESC, CTRL+q or CTRL+w; a single w
		   writes the grid state */
		case SDLK_w:
			if ((event->keysym.mod & KMOD_CTRL) != 0) {
				*loop = false;
			} else {
				_output_grid(gw->grid, out_file, out_file_format);
			}
			break;
		case SDLK_q:
			if ((event->keysym.mod & KMOD_CTRL) == 0) {
				break;
			}
		case SDLK_ESCAPE:
			*loop = false;
			break;
		case SDLK_c:
			clear_grid(gw->grid);
			break;
		case SDLK_h:
			_print_help();
			break;
	}
}
static void _handle_event(const SDL_Event *event, struct grid_window *gw,
                         bool *loop, bool *mdown, bool *play,
                         int *last_x, int *last_y, const char *repr,
                         enum grid_format repr_format, const char *out_file,
                         enum grid_format out_file_format) {
	switch (event->type) {
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT) {
			*mdown = true;
			get_cell_loc(gw, event->button.x, event->button.y, &gw->sel_col,
			           &gw->sel_row);
			_handle_mouse_on_cell(gw, last_x, last_y);
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if (event->button.button == SDL_BUTTON_LEFT) {
			*mdown = false;
		}
		break;
	case SDL_MOUSEMOTION:
		get_cell_loc(gw, event->motion.x, event->motion.y, &gw->sel_col,
		             &gw->sel_row);
		if (*mdown && (gw->sel_col != *last_x || gw->sel_row != *last_y)) {
			_handle_mouse_on_cell(gw, last_x, last_y);
		}
		break;
	case SDL_KEYDOWN:
		_handle_key_event(&event->key, gw, loop, play, repr, repr_format,
		                  out_file, out_file_format);
		break;
	case SDL_QUIT:
		*loop = false;
		break;
	}
}

void run_app(struct grid_window *gw, unsigned int update_rate,
             const char *repr, enum grid_format repr_format,
             const char *out_file, enum grid_format out_file_format) {
	Uint64 frame_start = SDL_GetPerformanceCounter();
	Uint64 frame_duration = SDL_GetPerformanceFrequency() / update_rate;

	bool loop = true;
	bool mdown = false;
	bool play = false;
	int last_x = INT_MIN;
	int last_y = INT_MIN;
	while (loop) {
		render_grid_window(gw);
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0) {
			_handle_event(&event, gw, &loop, &mdown, &play, &last_x, &last_y,
			            repr, repr_format, out_file, out_file_format);
		}

		while (frame_start + frame_duration <= SDL_GetPerformanceCounter()) {
			if (play) {
				update_grid(gw->grid);
			}
			frame_start += frame_duration;
		}
	}
}


void terminate_app(void) {
	SDL_Quit();
}
