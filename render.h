#ifndef RENDER_H
#define RENDER_H


#include <SDL2/SDL_render.h>

#include "life.h"


#define CELL_PIXELS    16

void renderBoard(Board *board, SDL_Renderer *renderer);

void toggleHoveredCell(Board *b, int mouse_x, int mouse_y);

#endif // RENDER_H
