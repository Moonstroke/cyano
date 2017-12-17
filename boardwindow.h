#ifndef RENDER_H
#define RENDER_H


#include <SDL2/SDL_render.h>
#include <stdbool.h>

#include "board.h"


#define WINDOW_FLAGS   SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS

#define REN_DEFT_FLAGS SDL_RENDERER_ACCELERATED
#define RENDERER_FLAGS(use_vsync) (use_vsync ? REN_DEFT_FLAGS | SDL_RENDERER_PRESENTVSYNC : REN_DEFT_FLAGS)

#define DEFAULT_CELLS_PIXELS 16

#define DEFAULT_UPDATE_RATE 25

#define DEFAULT_BORDER_WIDTH 1


typedef struct {
	Board *board;
	SDL_Window *win;
	SDL_Renderer *ren;
	unsigned int cell_pixels;
	unsigned int border_width;
	unsigned int sel_x, sel_y;
} BoardWindow;

BoardWindow *newBoardWindow(Board *board,
                            unsigned int cell_pixels,
                            unsigned int border_width,
                            const char *window_title,
                            bool use_vsync);

void freeBoardWindow(BoardWindow *self);


void renderBoardWindow(const BoardWindow *self);

void getHoverCoord(const BoardWindow *self, int *x, int *y);


#endif // RENDER_H
