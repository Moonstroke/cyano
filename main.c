#include <SDL2/SDL.h>

#include "log.h"
#include "life.h"


#define BOARD_WIDTH    40
#define BOARD_HEIGHT   30

#define CELL_PIXELS    16

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

	int mouse_x, mouse_y;
	bool loop = true;
	bool mouse_down = false;
	bool has_changed;
	bool play = false;
	SDL_Event event;
	SDL_RenderClear(ren);
	while(loop) {
		has_changed = false;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_MOUSEBUTTONDOWN:
					mouse_down = true;
					SDL_GetMouseState(&mouse_x, &mouse_y);
					toggleCell(&b, mouse_x / CELL_PIXELS, mouse_y / CELL_PIXELS);
					has_changed = true;
					break;
				case SDL_MOUSEBUTTONUP:
					mouse_down = false;
				case SDL_MOUSEMOTION:
					if(mouse_down) {
						SDL_GetMouseState(&mouse_x, &mouse_y);
						toggleCell(&b, mouse_x / CELL_PIXELS, mouse_y / CELL_PIXELS);
						has_changed = true;
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
							has_changed = true;
						// The window can be closed with ESC, CTRL+q or CTRL+w
						case SDLK_q:
						case SDLK_w:
							if(!(event.key.keysym.mod & KMOD_CTRL))
								break;
						case SDLK_ESCAPE:
							loop = false;
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
			has_changed = true;
		}
		if(has_changed) {
			SDL_RenderClear(ren);
			renderBoard(&b, ren, CELL_PIXELS);
			SDL_RenderPresent(ren);
		}
	}

	freeBoard(&b);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
