#ifndef RENDER_H
#define RENDER_H


#include <SDL2/SDL_render.h>

#include "life.h"


void renderBoard(Board *board, SDL_Renderer *renderer, int cell_pixels);

void toggleHoveredCell(Board *b, int mouse_x, int mouse_y, int cell_pixels);

#endif // RENDER_H
