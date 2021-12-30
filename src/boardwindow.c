#include "boardwindow.h"

#include <SDL2/SDL_mouse.h>



struct boardwindow *newBoardWindow(struct board *board, unsigned int c,
                                   unsigned int b, const char *t, bool v) {
	struct boardwindow *bw;
	unsigned int ww = board->w * (c + b) + b,
	             wh = board->h * (c + b) + b;
	SDL_Window *win;
	SDL_Renderer *ren;

	bw = malloc(sizeof(struct boardwindow));
	if(bw == NULL)
		return NULL;

	win = SDL_CreateWindow(t, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                       ww, wh, WINDOW_FLAGS);
	if(win == NULL)
		return NULL;

	ren = SDL_CreateRenderer(win, -1, RENDERER_FLAGS(v));
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

void freeBoardWindow(struct boardwindow *bw) {
	SDL_DestroyWindow(bw->win);
	SDL_DestroyRenderer(bw->ren);
	free(bw);
}

void updateBoardWindow(struct boardwindow *bw) {
	getHoverCoord(bw, &bw->sel_x, &bw->sel_y);
}

static inline void drawCell(SDL_Renderer *ren, SDL_Rect *rect, unsigned int i,
                            unsigned int j, unsigned int c, unsigned int border,
                            unsigned char r, unsigned char g, unsigned char b,
                            unsigned char a) {
	SDL_SetRenderDrawColor(ren, r, g, b, a);
	rect->x = (c + border) * i + border;
	rect->y = (c + border) * j + border;
	SDL_RenderFillRect(ren, rect);
}

void renderBoardWindow(const struct boardwindow *bw) {
	unsigned int w = bw->board->w,
	             h = bw->board->h,
	             c = bw->cell_pixels,
	             b = bw->border_width;

	unsigned int i, j;
	SDL_Rect r;

	SDL_SetRenderDrawColor(bw->ren, 127, 127, 127, 255);
	SDL_RenderClear(bw->ren);
	r.w = c;
	r.h = c;
	for(j = 0; j < h; ++j) {
		for(i = 0; i < w; ++i) {
			unsigned char ch = getBoardCell(bw->board, i, j) ? 0 : 255;
			drawCell(bw->ren, &r, i, j, c, b, ch, ch, ch, 255);
		}
	}
	SDL_SetRenderDrawBlendMode(bw->ren, SDL_BLENDMODE_BLEND);
	drawCell(bw->ren, &r, bw->sel_x, bw->sel_y, c, b, 127, 127, 127, 127);
	SDL_RenderPresent(bw->ren);
}

void getHoverCoord(const struct boardwindow *bw, int *x, int *y) {
	unsigned int c = bw->cell_pixels, b = bw->border_width;
	SDL_GetMouseState(x, y);
	*x = (*x - b) / (c + b);
	*y = (*y - b) / (c + b);
}
