#include <SDL2/SDL.h>

#include <clog.h>
#include "board.h"
#include "boardwindow.h"
#include "clop.h"
#include "timer.h"


int main(int argc, char **argv) {

	// Objects
	struct board b;
	struct boardwindow *bw;

	// Initializers parameters
	unsigned int board_width = DEFAULT_BOARD_WIDTH,
	             board_height = DEFAULT_BOARD_HEIGHT,
	             cell_pixels = DEFAULT_CELLS_PIXELS,
	             update_rate = DEFAULT_UPDATE_RATE,
	             border_width = DEFAULT_BORDER_WIDTH;
	bool use_vsync = false, wrap = false;
	const char *game_rules = DEFAULT_BOARD_RULES;

	// Main loop variables
	struct timer timer;
	int over_x, over_y,
	    last_x, last_y;
	bool loop = true;
	bool mdown = false;
	bool play = false;
	SDL_Event event;

	setvars(&board_width, &board_height, &cell_pixels, &update_rate,
	        &border_width, &use_vsync, &wrap, &game_rules);
	if(getvals(argc, argv, OPTSTRING, LONGOPTS) < 0) {
		fatal("Failure in command line options handling");
		return 1;
	}

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fatal("Could not load the SDL: %s", SDL_GetError());
		return 1;
	}

	initBoard(&b, board_width, board_height, wrap);
	setRules(&b, game_rules);

	bw = newBoardWindow(&b, cell_pixels, border_width, "SDL Game of Life",
	                    use_vsync);
	if(bw == NULL) {
		fatal("Could not create the game window: %s", SDL_GetError());
		return 1;
	}

	resetTimer(&timer);
	timer.delay = 1000. / (double)update_rate;

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
					break;
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
