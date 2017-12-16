#include "boardwindow.h"

#include <SDL2/SDL_mouse.h>

#include "log.h"


static inline void getWinDim(const Board *const b, const unsigned int c, unsigned int *const ww, unsigned int *const wh) {
	*ww = b->w * c;
	*wh = b->h * c;
}

BoardWindow *newBoardWindow(Board *const b, const unsigned int c, const char *const t, bool v) {
	BoardWindow *bw = malloc(sizeof(BoardWindow));
	if(bw == NULL)
		return NULL;
	unsigned int ww, wh;
	getWinDim(b, c, &ww, &wh);

	SDL_Window *const win = SDL_CreateWindow(t, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ww, wh, WINDOW_FLAGS);
	if(win == NULL)
		return NULL;

	SDL_Renderer *const ren = SDL_CreateRenderer(win, -1, RENDERER_FLAGS(v));
	if(ren == NULL)
		return NULL;

	bw->board = b;
	bw->win = win;
	bw->ren = ren;
	bw->cell_pixels = c;
	bw->sel_x = bw->sel_y = -1;

	return bw;
}

void freeBoardWindow(BoardWindow *const bw) {
	SDL_DestroyWindow(bw->win);
	SDL_DestroyRenderer(bw->ren);
	free(bw);
}

void renderBoardWindow(const BoardWindow *const bw) {
	SDL_RenderClear(bw->ren);
	const unsigned int w = bw->board->w,
	                   h = bw->board->h,
	                   c = bw->cell_pixels;

	unsigned int i, j;
	SDL_Rect r;
	r.w = c;
	r.h = c;
	for(j = 0; j < h; ++j) {
		for(i = 0; i < w; ++i) {
			const unsigned char ch = *getCell(bw->board, i, j) ? 0 : 255;
			SDL_SetRenderDrawColor(bw->ren, ch, ch, ch, 255);
			r.x = c * i;
			r.y = c * j;
			SDL_RenderFillRect(bw->ren, &r);
		}
	}
	SDL_RenderPresent(bw->ren);
}

void getHoverCoord(const BoardWindow *const bw, int *const x, int *y) {
	const unsigned int d = bw->cell_pixels;
	SDL_GetMouseState(x, y);
	*x /= d;
	*y /= d;
}
