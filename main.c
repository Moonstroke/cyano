#include <SDL2/SDL.h>

#include "log.h"
#include "life.h"
#include "render.h"
#include "clop.h"


#define WINDOW_FLAGS   SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC

int main(const int argc, const char *const argv[]) {

	unsigned int board_width = DEFAULT_BOARD_WIDTH,
	             board_height = DEFAULT_BOARD_HEIGHT,
	             cell_pixels = DEFAULT_CELLS_PIXELS;

	setvars(&board_width, &board_height, &cell_pixels);
	getvals(argc, argv, OPTSTRING);

	debug("board = (%dx%d)x%d", board_width, board_height, cell_pixels);
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fatal("Could not load SDL: %s", SDL_GetError());
		return 1;
	}
	SDL_Window *win = SDL_CreateWindow("SDL Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, board_width * cell_pixels, board_height * cell_pixels, WINDOW_FLAGS);
	if(win == NULL) {
		fatal("Could not create SDL window: %s", SDL_GetError());
		return 1;
	}
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, RENDERER_FLAGS);
	if(ren == NULL) {
		fatal("Could not create SDL renderer: %s", SDL_GetError());
		return 1;
	}
	

	Board b;
	initBoard(&b, board_width, board_height);

	int mx, my,
	    last_x, last_y;
	bool loop = true;
	bool mdown = false;
	bool play = false;
	SDL_Event event;
	while(loop) {
		renderBoard(&b, ren, cell_pixels);
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_MOUSEBUTTONDOWN:
					mdown = true;
					toggleHoveredCell(&b, mx, my, cell_pixels);
					break;
				case SDL_MOUSEBUTTONUP:
					mdown = false;
				case SDL_MOUSEMOTION:
					SDL_GetMouseState(&mx, &my);
					if(mdown) {
						const int cx = mx / cell_pixels, cy = my / cell_pixels;
						if(cx != last_x || cy != last_y) {
							toggleHoveredCell(&b, mx, my, cell_pixels);
							last_x = cx;
							last_y = cy;
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
								nextGen(&b);
						// The window can be closed with ESC, CTRL+q or CTRL+w
						case SDLK_q:
						case SDLK_w:
							if(!(event.key.keysym.mod & KMOD_CTRL))
								break;
						case SDLK_ESCAPE:
							loop = false;
							break;
						case SDLK_c:
							clear(&b);
							break;
					}
					break;
				case SDL_QUIT:
					loop = false;
					break;
			}
		}
		if(play) {
			nextGen(&b);
		}
	}

	freeBoard(&b);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
