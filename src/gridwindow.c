#include "gridwindow.h"

#include <SDL2/SDL_mouse.h>



int initGridWindow(struct gridwindow *gw, struct grid *grid,
                    unsigned int cell_pixels, unsigned int border_width,
                    const char *title, bool use_vsync) {
	unsigned int winWidth = grid->w * (cell_pixels + border_width)
	                        + border_width,
	             winHeight = grid->h * (cell_pixels + border_width)
	                         + border_width;

	Uint32 win_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_FOCUS
	                                    | SDL_WINDOW_INPUT_FOCUS;
	gw->win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
	                           SDL_WINDOWPOS_CENTERED, winWidth, winHeight,
	                           win_flags);
	if (gw->win == NULL) {
		strncpy(gw->error_msg, SDL_GetError(), sizeof gw->error_msg);
		return -1;
	}
	Uint32 ren_flags = SDL_RENDERER_ACCELERATED;
	if (use_vsync) {
		ren_flags |= SDL_RENDERER_PRESENTVSYNC;
	}
	gw->ren = SDL_CreateRenderer(gw->win, -1, ren_flags);
	if (gw->ren == NULL) {
		strncpy(gw->error_msg, SDL_GetError(), sizeof gw->error_msg);
		return -2;
	}
	gw->grid = grid;
	gw->cell_pixels = cell_pixels;
	gw->sel_x = gw->sel_y = -1;
	gw->border_width = border_width;
	gw->error_msg[0] = '\0';

	return 0;
}


void freeGridWindow(struct gridwindow *gw) {
	SDL_DestroyWindow(gw->win);
	SDL_DestroyRenderer(gw->ren);
}


static inline void _drawCell(SDL_Renderer *ren, SDL_Rect *rect, unsigned int i,
                             unsigned int j, unsigned int c,
                             unsigned int border, unsigned char r,
                             unsigned char g, unsigned char b,
                             unsigned char a) {
	SDL_SetRenderDrawColor(ren, r, g, b, a);
	rect->x = (c + border) * i + border;
	rect->y = (c + border) * j + border;
	SDL_RenderFillRect(ren, rect);
}

void renderGridWindow(const struct gridwindow *gw) {
	unsigned int w = gw->grid->w,
	             h = gw->grid->h,
	             c = gw->cell_pixels,
	             b = gw->border_width;

	unsigned int i, j;
	SDL_Rect r;

	SDL_SetRenderDrawColor(gw->ren, 127, 127, 127, 255);
	SDL_RenderClear(gw->ren);
	r.w = c;
	r.h = c;
	for (j = 0; j < h; ++j) {
		for (i = 0; i < w; ++i) {
			unsigned char ch = getGridCell(gw->grid, i, j) ? 0 : 255;
			_drawCell(gw->ren, &r, i, j, c, b, ch, ch, ch, 255);
		}
	}
	SDL_SetRenderDrawBlendMode(gw->ren, SDL_BLENDMODE_BLEND);
	if (gw->sel_x >= 0 && gw->sel_y >= 0) {
		_drawCell(gw->ren, &r, gw->sel_x, gw->sel_y, c, b, 127, 127, 127, 127);
	}
	SDL_RenderPresent(gw->ren);
}


void getCellLoc(const struct gridwindow *gw, int x, int y, int *i, int *j) {
	/* Remove offset for up and left border */
	x -= gw->border_width;
	y -= gw->border_width;
	unsigned int step = gw->cell_pixels + gw->border_width;
	if (x % step < gw->border_width) {
		/* Hovering a vertical border */
		*i = -1;
	} else {
		*i = x / step;
	}
	if (y % step < gw->border_width) {
		/* Hovering a horizontal border */
		*j = -1;
	} else {
		*j = y / step;
	}
}

void getHoveredCellLoc(const struct gridwindow *gw, int *i, int *j) {
	int x, y;
	SDL_GetMouseState(&x, &y);
	getCellLoc(gw, x, y, i, j);
}
