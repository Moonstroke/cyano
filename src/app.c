#include "app.h"

#include <SDL2/SDL.h>
#include <stdio.h> /* for fprintf, stderr, fputs */

#include "grid.h"
#include "gridwindow.h"
#include "file_io.h" /* for writeFile */
#include "timer.h"



int initApp(void) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Could not load the SDL: %s\n", SDL_GetError());
		return -1;
	}

	return 0;
}


static void _handleMouseOnCell(struct gridwindow *gw, int *last_x,
                               int *last_y) {
	if (gw->sel_x > 0 && gw->sel_y > 0) {
		toggleCell(gw->grid, gw->sel_x, gw->sel_y);
		*last_x = gw->sel_x;
		*last_y = gw->sel_y;
	}
}

static inline void _resetGrid(struct grid *grid, const char *repr,
                              enum grid_format format, bool *play, bool *loop) {
	if (repr != NULL) {
		if (*play) {
			*play = false;
		}
		bool wrap = grid->wrap;
		freeGrid(grid);
		if (loadGrid(grid, repr, format, wrap) < 0) {
			fputs("Error while resetting the grid\n", stderr);
			*loop = false;
		}
	}
}

static inline int _outputGrid(const struct grid *grid, const char *out_file) {
	char *repr = getGridRepr(grid);
	if (repr == NULL) {
		return -1;
	}
	int rc = writeFile(out_file, repr);
	free(repr);
	return rc;
}

static inline int _printHelp(void) {
	fputs("Interface usage:\n"
	      " - Using the mouse\n"
	      "Move the mouse cursor to highlight a cell, and click the left\n"
	      " button to toggle\nits state.\n"
	      " - Using the keyboard\n"
	      "The following keys or key combinations have an action defined:\n"
	      "    Key    Action\n"
	      "     C     Clear the grid\n"
	      "     H     Display this message\n"
	      " Ctrl + Q  Quit the program\n"
	      "     R     Reset the grid to the input file\n"
	      "     T     Toggle the state of the highlighted cell\n"
	      "     W     Write the state of the grid to the output file in plain"
	      " text\n"
	      " Ctrl + W  Quit the program\n"
	      "   Space   Toggle pause mode\n"
	      "   Enter   When paused, evolve the grid by one generation\n"
	      "    Esc    Quit the program\n"
	      "Arrow keys Move the highlight by one cell in the key's direction\n",
		  stdout);
}

static void _handleEvent(const SDL_Event *event, struct gridwindow *gw,
                        bool *loop, bool *mdown, bool *play,
                        int *last_x, int *last_y, const char *repr,
                        enum grid_format format, const char *out_file) {
	switch (event->type) {
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT) {
			*mdown = true;
			getCellLoc(gw, event->button.x, event->button.y, &gw->sel_x,
			           &gw->sel_y);
			_handleMouseOnCell(gw, last_x, last_y);
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if (event->button.button == SDL_BUTTON_LEFT) {
			*mdown = false;
		}
		break;
	case SDL_MOUSEMOTION:
		getCellLoc(gw, event->motion.x, event->motion.y, &gw->sel_x,
		             &gw->sel_y);
		if (*mdown) {
			if ((gw->sel_x != *last_x) || (gw->sel_y != *last_y)) {
				_handleMouseOnCell(gw, last_x, last_y);
			}
		}
		break;
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym) {
			case SDLK_SPACE:
				*play = !*play;
				break;
			case SDLK_RETURN:
				if (!*play) {
					updateGrid(gw->grid);
				}
				break;
			case SDLK_UP:
				if (--gw->sel_y < 0) {
					gw->sel_y = gw->grid->wrap ? gw->grid->h - 1 : 0;
				}
				break;
			case SDLK_DOWN:
				if (++gw->sel_y >= (signed) gw->grid->h) {
					gw->sel_y = gw->grid->wrap ? 0 : gw->grid->h - 1;
				}
				break;
			case SDLK_LEFT:
				if (--gw->sel_x < 0) {
					gw->sel_x = gw->grid->wrap ? gw->grid->w - 1 : 0;
				}
				break;
			case SDLK_RIGHT:
				if (++gw->sel_x >= (signed) gw->grid->w) {
					gw->sel_x = gw->grid->wrap ? 0 : gw->grid->w - 1;
				}
				break;
			case SDLK_t:
				toggleCell(gw->grid, gw->sel_x, gw->sel_y);
				break;
			case SDLK_r:
				_resetGrid(gw->grid, repr, format, play, loop);
				break;
			/* The window can be closed with ESC, CTRL+q or CTRL+w; a single w
			   writes the grid state */
			case SDLK_w:
				if (event->key.keysym.mod & KMOD_CTRL) {
					*loop = false;
				} else {
					_outputGrid(gw->grid, out_file);
				}
			case SDLK_q:
				if (!(event->key.keysym.mod & KMOD_CTRL)) {
					break;
				}
			case SDLK_ESCAPE:
				*loop = false;
				break;
			case SDLK_c:
				clearGrid(gw->grid);
				break;
			case SDLK_h:
				_printHelp();
				break;
		}
		break;
	case SDL_QUIT:
		*loop = false;
		break;
	}
}

void runApp(struct gridwindow *gw, unsigned int update_rate, bool use_vsync,
            const char *repr, enum grid_format format, const char *out_file) {
	struct timer timer;
	resetTimer(&timer);
	timer.delay = 1000. / (double) update_rate;

	bool loop = true;
	bool mdown = false;
	bool play = false;
	while (loop) {
		int last_x, last_y;
		startTimer(&timer);
		renderGridWindow(gw);
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			_handleEvent(&event, gw, &loop, &mdown, &play, &last_x, &last_y,
			            repr, format, out_file);
		}

		if (play) {
			updateGrid(gw->grid);
		}

		unsigned int remTime = getRemainingTime(&timer);
		if (!use_vsync && remTime > 0) {
			SDL_Delay(remTime);
		}
	}
}


void terminateApp(void) {
	SDL_Quit();
}
