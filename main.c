#include <SDL2/SDL.h>

#include "log.h"
#include "board.h"
#include "boardwindow.h"
#include "clop.h"
#include "timer.h"


int main(const int argc, const char *const argv[]) {

	unsigned int board_width = DEFAULT_BOARD_WIDTH,
	             board_height = DEFAULT_BOARD_HEIGHT,
	             cell_pixels = DEFAULT_CELLS_PIXELS,
	             update_rate = DEFAULT_UPDATE_RATE,
	             border_width = DEFAULT_BORDER_WIDTH;
	bool use_vsync = false, wrap = false;

	setvars(&board_width, &board_height, &cell_pixels, &update_rate, &border_width, &use_vsync, &wrap);
	if(!getvals(argc, argv, OPTSTRING, LONGOPTS)) {
		fatal("Failure in command line options handling");
		return 1;
	}

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fatal("Could not load the SDL: %s", SDL_GetError());
		return 1;
	}

	Board b;
	initBoard(&b, board_width, board_height, wrap);

	BoardWindow *bw = newBoardWindow(&b, cell_pixels, border_width, "SDL Game of Life", use_vsync);
	if(bw == NULL) {
		fatal("Could not create the game window: %s", SDL_GetError());
		return 1;
	}

	Timer timer;
	resetTimer(&timer);
	setDelay(&timer, 1000. / (double)update_rate);

	int over_x, over_y,
	    last_x, last_y;
	bool loop = true;
	bool mdown = false;
	bool play = false;
	SDL_Event event;
	while(loop) {
		unsigned int remTime;
		startTimer(&timer);
		updateBoardWindow(bw);
		renderBoardWindow(bw);
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_MOUSEBUTTONDOWN:
					mdown = true;
					getHoverCoord(bw, &over_x, &over_y);
					toggleCell(&b, over_x, over_y);
					last_x = over_x;
					last_y = over_y;
					break;
				case SDL_MOUSEBUTTONUP:
					mdown = false;
				case SDL_MOUSEMOTION:
					if(mdown) {
						getHoverCoord(bw, &over_x, &over_y);
						if((over_x != last_x) || (over_y != last_y)) {
							toggleCell(&b, over_x, over_y);
							last_x = over_x;
							last_y = over_y;
						}
					}
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_SPACE:
							play = !play;
							break;
						case SDLK_RETURN:
							if(!play)
								updateBoard(&b);
						// The window can be closed with ESC, CTRL+q or CTRL+w
						case SDLK_q:
						case SDLK_w:
							if(!(event.key.keysym.mod & KMOD_CTRL))
								break;
						case SDLK_ESCAPE:
							loop = false;
							break;
						case SDLK_c:
							clearBoard(&b);
							break;
					}
					break;
				case SDL_QUIT:
					loop = false;
					break;
			}
		}

		if(play) {
			updateBoard(&b);
		}

		remTime = getRemainingTime(&timer);
		if(!use_vsync && remTime > 0) {
			SDL_Delay(remTime);
		}
	}

	freeBoard(&b);
	freeBoardWindow(bw);
	SDL_Quit();
	return 0;
}
