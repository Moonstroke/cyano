/* SPDX-License-Identifier: CECILL-2.1 */
#include "gridwindow.h"

#include <stdint.h> /* for uint32_t */
#include <string.h> /* for strncpy */
#include <SDL2/SDL_mouse.h> /* for SDL_GetMouseState */



#ifdef USE_VSYNC
# define RENDERER_FLAGS SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
#else
# define RENDERER_FLAGS SDL_RENDERER_ACCELERATED
#endif


static uint32_t icon_data[] = {
#if ICONSIZE == 16
# include "icon16.dat"
#elif ICONSIZE == 64
# include "icon64.dat"
#endif
};


int init_grid_window(struct grid_window *grid_win, struct grid *grid,
                     unsigned int cell_pixels, unsigned int border_width,
                     const char *title) {
	grid_win->grid = grid;
	grid_win->cell_pixels = cell_pixels;
	grid_win->border_width = border_width;
	unsigned int win_width = GRID_SIZE_TO_WIN_SIZE(grid_win, grid->width);
	unsigned int win_height = GRID_SIZE_TO_WIN_SIZE(grid_win, grid->height);

	Uint32 win_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_FOCUS
	                                    | SDL_WINDOW_INPUT_FOCUS;
	grid_win->win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
	                           SDL_WINDOWPOS_CENTERED, win_width, win_height,
	                           win_flags);
	if (grid_win->win == NULL) {
		strncpy(grid_win->error_msg, SDL_GetError(),
		        sizeof grid_win->error_msg);
		return -__LINE__;
	}
	grid_win->ren = SDL_CreateRenderer(grid_win->win, -1, RENDERER_FLAGS);
	if (grid_win->ren == NULL) {
		strncpy(grid_win->error_msg, SDL_GetError(),
		        sizeof grid_win->error_msg);
		return -__LINE__;
	}
	grid_win->sel_col = grid_win->sel_row = -1;
	grid_win->error_msg[0] = '\0';

	SDL_Surface *icon = SDL_CreateRGBSurfaceFrom(icon_data, ICONSIZE, ICONSIZE,
	                                             32,
	                                             ICONSIZE * sizeof icon_data[0],
	                                             0xff000000, 0x00ff0000,
	                                             0x0000ff00, 0x000000ff);
	if (icon != NULL) {
		SDL_SetWindowIcon(grid_win->win, icon);
		SDL_FreeSurface(icon);
	}

	return 0;
}


void free_grid_window(struct grid_window *grid_win) {
	SDL_DestroyRenderer(grid_win->ren);
	SDL_DestroyWindow(grid_win->win);
}


static inline void _draw_cell(SDL_Renderer *ren, SDL_Rect *rect,
                             unsigned int row, unsigned int col,
                             unsigned int cell_width,
                             unsigned int border_width, SDL_Color color) {
	SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
	rect->x = (cell_width + border_width) * col + border_width;
	rect->y = (cell_width + border_width) * row + border_width;
	SDL_RenderFillRect(ren, rect);
}

void render_grid_window(const struct grid_window *grid_win) {
	unsigned int grid_width = grid_win->grid->width;
	unsigned int grid_height = grid_win->grid->height;
	unsigned int cell_width = grid_win->cell_pixels;
	unsigned int border_width = grid_win->border_width;

	SDL_Rect rect;

	SDL_SetRenderDrawColor(grid_win->ren, 127, 127, 127, 255);
	SDL_RenderClear(grid_win->ren);
	rect.w = cell_width;
	rect.h = cell_width;
	uint8_t cell_color[] = {
		[DEAD] = 255,
		[ALIVE] = 0
	};
	for (unsigned int row = 0; row < grid_height; ++row) {
		for (unsigned int col = 0; col < grid_width; ++col) {
			uint8_t ch = cell_color[get_grid_cell(grid_win->grid, row, col)];
			_draw_cell(grid_win->ren, &rect, row, col, cell_width,
					   border_width, (SDL_Color) {ch, ch, ch, 255});
		}
	}
	SDL_SetRenderDrawBlendMode(grid_win->ren, SDL_BLENDMODE_BLEND);
	if (grid_win->sel_col >= 0 && grid_win->sel_row >= 0) {
		_draw_cell(grid_win->ren, &rect, grid_win->sel_row, grid_win->sel_col,
		           cell_width, border_width, (SDL_Color) {127, 127, 127, 127});
	}
	SDL_RenderPresent(grid_win->ren);
}


void get_cell_loc(const struct grid_window *grid_win, int window_x,
                  int window_y, int *cell_col, int *cell_row) {
	/* Remove offset for up and left border */
	window_x -= grid_win->border_width;
	window_y -= grid_win->border_width;
	unsigned int step = grid_win->cell_pixels + grid_win->border_width;
	if (window_x % step < grid_win->border_width) {
		/* Hovering a vertical border */
		*cell_col = -1;
	} else {
		*cell_col = window_x / step;
	}
	if (window_y % step < grid_win->border_width) {
		/* Hovering a horizontal border */
		*cell_row = -1;
	} else {
		*cell_row = window_y / step;
	}
}

void get_hovered_cell_loc(const struct grid_window *grid_win, int *cell_col,
                          int *cell_row) {
	int mouse_x;
	int mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);
	get_cell_loc(grid_win, mouse_x, mouse_y, cell_col, cell_row);
}
