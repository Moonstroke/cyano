#ifndef RENDER_H
#define RENDER_H


#include <SDL2/SDL_render.h>

#include "life.h"


#define DEFAULT_CELLS_PIXELS 16

void renderBoard(Board *board, SDL_Renderer *renderer, int cell_pixels);

void getHoverCoord(int cell_pixels, int *x, int *y);

void getWindowDim(unsigned int board_width, unsigned int board_height, unsigned int cells_dim, unsigned int *win_width, unsigned int *win_height);


#endif // RENDER_H
