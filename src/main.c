#include <SDL2/SDL.h>

#include <clog.h>
#include "board.h"
#include "boardwindow.h"
#include "clop.h"
#include "timer.h"



static void handleEvent(const SDL_Event *event, struct boardwindow *bw,
                        bool *loop, bool *mdown, bool *play,
                        int *last_x, int *last_y) {
	switch (event->type) {
	case SDL_MOUSEBUTTONDOWN:
		*mdown = true;
		toggleCell(bw->board, bw->sel_x, bw->sel_y);
		*last_x = bw->sel_x;
		*last_y = bw->sel_y;
		break;
	case SDL_MOUSEBUTTONUP:
		*mdown = false;
		break;
	case SDL_MOUSEMOTION:
		getHoverCoord(bw, &bw->sel_x, &bw->sel_y);
		if (*mdown) {
			if ((bw->sel_x != *last_x) || (bw->sel_y != *last_y)) {
				toggleCell(bw->board, bw->sel_x, bw->sel_y);
				*last_x = bw->sel_x;
				*last_y = bw->sel_y;
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


int main(int argc, char **argv) {

	unsigned int board_width = DEFAULT_BOARD_WIDTH,
	             board_height = DEFAULT_BOARD_HEIGHT,
	             cell_pixels = DEFAULT_CELLS_PIXELS,
	             update_rate = DEFAULT_UPDATE_RATE,
	             border_width = DEFAULT_BORDER_WIDTH;
	bool use_vsync = false, wrap = false;
	const char *game_rules = DEFAULT_BOARD_RULES;

	setvars(&board_width, &board_height, &cell_pixels, &update_rate,
	        &border_width, &use_vsync, &wrap, &game_rules);
	if (getvals(argc, argv, OPTSTRING, LONGOPTS) < 0) {
		fatal("Failure in command line options handling");
		return 1;
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fatal("Could not load the SDL: %s", SDL_GetError());
		return 1;
	}

	struct board b;
	initBoard(&b, board_width, board_height, wrap);
	setRules(&b, game_rules);

	struct boardwindow *bw = newBoardWindow(&b, cell_pixels, border_width,
	                                        "SDL Game of Life", use_vsync);
	if (bw == NULL) {
		fatal("Could not create the game window: %s", SDL_GetError());
		return 1;
	}

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
			handleEvent(&event, bw, &loop, &mdown, &play, &last_x, &last_y);
		}

		if (play) {
			updateBoard(&b);
		}

		unsigned int remTime = getRemainingTime(&timer);
		if (!use_vsync && remTime > 0) {
			SDL_Delay(remTime);
		}
	}

	freeBoard(&b);
	freeBoardWindow(bw);
	SDL_Quit();
	return 0;
}
