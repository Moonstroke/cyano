#ifndef RENDER_H
#define RENDER_H


#include <SDL2/SDL_render.h>

#include "life.h"


#define DEFAULT_CELLS_PIXELS 16

void renderBoard(Board *board, SDL_Renderer *renderer, int cell_pixels);

void getHoverCoord(int cell_pixels, int *x, int *y);

#endif // RENDER_H
