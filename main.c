#include <SDL2/SDL.h>

#include "log.h"
#include "life.h"
#include "render.h"


#define BOARD_WIDTH    80
#define BOARD_HEIGHT   45

#define WINDOW_HEIGHT  BOARD_HEIGHT * CELL_PIXELS
#define WINDOW_WIDTH   BOARD_WIDTH * CELL_PIXELS
#define WINDOW_FLAGS   SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC

int main(void) {

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fatal("Could not load SDL: %s", SDL_GetError());
		return 1;
	}
	SDL_Window *win = SDL_CreateWindow("SDL Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS);
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
	initBoard(&b, BOARD_WIDTH, BOARD_HEIGHT);

	int mx, my,
	    last_x, last_y;
	bool loop = true;
	bool mdown = false;
	bool play = false;
	SDL_Event event;
	while(loop) {
		renderBoard(&b, ren);
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_MOUSEBUTTONDOWN:
					mdown = true;
					toggleHoveredCell(&b, mx, my);
					break;
				case SDL_MOUSEBUTTONUP:
					mdown = false;
				case SDL_MOUSEMOTION:
					SDL_GetMouseState(&mx, &my);
					if(mdown) {
						const int cx = mx / CELL_PIXELS, cy = my / CELL_PIXELS;
						if(cx != last_x || cy != last_y) {
							toggleHoveredCell(&b, mx, my);
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
