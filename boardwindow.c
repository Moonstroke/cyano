#include "boardwindow.h"

#include <SDL2/SDL_mouse.h>

#include "log.h"


BoardWindow *newBoardWindow(Board *const board, const unsigned int c, const unsigned int b, const char *const t, bool v) {
	BoardWindow *bw = malloc(sizeof(BoardWindow));
	if(bw == NULL)
		return NULL;
	const unsigned int ww = board->w * (c + b) + b,
	                   wh = board->h * (c + b) + b;

	SDL_Window *const win = SDL_CreateWindow(t, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ww, wh, WINDOW_FLAGS);
	if(win == NULL)
		return NULL;

	SDL_Renderer *const ren = SDL_CreateRenderer(win, -1, RENDERER_FLAGS(v));
	if(ren == NULL)
		return NULL;

	bw->board = board;
	bw->win = win;
	bw->ren = ren;
	bw->cell_pixels = c;
	bw->sel_x = bw->sel_y = -1;
	bw->border_width = b;

	return bw;
}

void freeBoardWindow(BoardWindow *const bw) {
	SDL_DestroyWindow(bw->win);
	SDL_DestroyRenderer(bw->ren);
	free(bw);
}

void renderBoardWindow(const BoardWindow *const bw) {
	SDL_SetRenderDrawColor(bw->ren, 127, 127, 127, 255);
	SDL_RenderClear(bw->ren);
	const unsigned int w = bw->board->w,
	                   h = bw->board->h,
	                   c = bw->cell_pixels,
	                   b = bw->border_width;

	unsigned int i, j;
	SDL_Rect r;
	r.w = c;
	r.h = c;
	for(j = 0; j < h; ++j) {
		for(i = 0; i < w; ++i) {
			const unsigned char ch = *getCell(bw->board, i, j) ? 0 : 255;
			SDL_SetRenderDrawColor(bw->ren, ch, ch, ch, 255);
			r.x = (c + b) * i + b;
			r.y = (c + b) * j + b;
			SDL_RenderFillRect(bw->ren, &r);
		}
	}
	SDL_RenderPresent(bw->ren);
}

void getHoverCoord(const BoardWindow *const bw, int *const x, int *y) {
	const unsigned int c = bw->cell_pixels, b = bw->border_width;
	SDL_GetMouseState(x, y);
	*x = (*x - b) / (c + b);
	*y = (*y - b) / (c + b);
}
