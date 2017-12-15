#ifndef RENDER_H
#define RENDER_H


#include <SDL2/SDL_render.h>

#include "life.h"


#define WINDOW_FLAGS   SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC

#define DEFAULT_CELLS_PIXELS 16

typedef struct {
	Board *board;
	SDL_Window *win;
	SDL_Renderer *ren;
	unsigned int cell_pixels;
	unsigned int sel_x, sel_y;
	unsigned char padding[4];
} BoardWindow;

BoardWindow *newBoardWindow(Board *board, unsigned int cell_pixels, const char *window_title);

void freeBoardWindow(BoardWindow *self);


void renderBoardWindow(const BoardWindow *self);

void getHoverCoord(const BoardWindow *self, int *x, int *y);


#endif // RENDER_H
