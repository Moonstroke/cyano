#include "render.h"


void renderBoard(Board *const b, SDL_Renderer *const ren) {
	const unsigned int w = b->w, h = b->h;
	unsigned int i, j;
	SDL_Rect r;
	for(j = 0; j < h; ++j) {
		for(i = 0; i < w; ++i) {
			const unsigned char c = getCell(b, i, j) ? 0 : 255;
			SDL_SetRenderDrawColor(ren, c, c, c, 255);
			r.x = CELL_PIXELS * i;
			r.y = CELL_PIXELS * j;
			r.w = CELL_PIXELS;
			r.h = CELL_PIXELS;
			SDL_RenderFillRect(ren, &r);
		}
	}
}

void toggleHoveredCell(Board *const b, const int x, const int y) {
	toggleCell(b, x / CELL_PIXELS, y / CELL_PIXELS);
}
