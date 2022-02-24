#include "app.h"

#include <SDL2/SDL.h>

#include "board.h"
#include "boardwindow.h"
#include "timer.h"



int initApp(void) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Could not load the SDL: %s\n", SDL_GetError());
		return -1;
	}

	return 0;
}


static void handleMouseOnCell(struct boardwindow *bw, int *last_x,
                              int *last_y) {
	if (bw->sel_x > 0 && bw->sel_y > 0) {
		toggleCell(bw->board, bw->sel_x, bw->sel_y);
		*last_x = bw->sel_x;
		*last_y = bw->sel_y;
	}
}

static inline void resetBoard(struct board *board, const char *repr, bool *play,
                             bool *loop) {
	if (repr != NULL) {
		if (*play) {
			*play = false;
		}
		bool wrap = board->wrap;
		freeBoard(board);
		if (loadBoard(board, repr, wrap) < 0) {
			fputs("Error while resetting the board\n", stderr);
			*loop = false;
		}
	}
}

static void handleEvent(const SDL_Event *event, struct boardwindow *bw,
                        bool *loop, bool *mdown, bool *play,
                        int *last_x, int *last_y, const char *repr,
                        const char *out_file) {
	switch (event->type) {
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT) {
			*mdown = true;
			getCellLoc(bw, event->button.x, event->button.y, &bw->sel_x,
			           &bw->sel_y);
			handleMouseOnCell(bw, last_x, last_y);
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if (event->button.button == SDL_BUTTON_LEFT) {
			*mdown = false;
		}
		break;
	case SDL_MOUSEMOTION:
		getCellLoc(bw, event->motion.x, event->motion.y, &bw->sel_x,
		             &bw->sel_y);
		if (*mdown) {
			if ((bw->sel_x != *last_x) || (bw->sel_y != *last_y)) {
				handleMouseOnCell(bw, last_x, last_y);
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
					updateBoard(bw->board);
				}
				break;
			case SDLK_UP:
				if (--bw->sel_y < 0) {
					bw->sel_y = bw->board->wrap ? bw->board->h - 1 : 0;
				}
				break;
			case SDLK_DOWN:
				if (++bw->sel_y >= (signed) bw->board->h) {
					bw->sel_y = bw->board->wrap ? 0 : bw->board->h - 1;
				}
				break;
			case SDLK_LEFT:
				if (--bw->sel_x < 0) {
					bw->sel_x = bw->board->wrap ? bw->board->w - 1 : 0;
				}
				break;
			case SDLK_RIGHT:
				if (++bw->sel_x >= (signed) bw->board->w) {
					bw->sel_x = bw->board->wrap ? 0 : bw->board->w - 1;
				}
				break;
			case SDLK_t:
				toggleCell(bw->board, bw->sel_x, bw->sel_y);
				break;
			case SDLK_r:
				resetBoard(bw->board, repr, play, loop);
				break;
			/* The window can be closed with ESC, CTRL+q or CTRL+w */
			case SDLK_q:
			case SDLK_w:
				if (!(event->key.keysym.mod & KMOD_CTRL)) {
					break;
				}
			case SDLK_ESCAPE:
				*loop = false;
				break;
			case SDLK_c:
				clearBoard(bw->board);
				break;
		}
		break;
	case SDL_QUIT:
		*loop = false;
		break;
	}
}

void runApp(struct boardwindow *bw, unsigned int update_rate, bool use_vsync,
            const char *repr, const char *out_file) {
	struct timer timer;
	resetTimer(&timer);
	timer.delay = 1000. / (double)update_rate;

	bool loop = true;
	bool mdown = false;
	bool play = false;
	while (loop) {
		int last_x, last_y;
		startTimer(&timer);
		renderBoardWindow(bw);
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			handleEvent(&event, bw, &loop, &mdown, &play, &last_x, &last_y,
			            repr, out_file);
		}

		if (play) {
			updateBoard(bw->board);
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
