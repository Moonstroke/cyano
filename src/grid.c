#include "grid.h"


#include <stdlib.h> /* for calloc, NULL, free */
#include <string.h> /* for strchr, memset */

#include "bits.h"



int init_grid(struct grid *g, unsigned int w, unsigned int h, bool wrap) {
	g->w = w;
	g->h = h;

	char *cells = calloc(NUM_OCTETS(w * h), 1);
	g->cells = cells;
	g->wrap = wrap;
	return cells == NULL ? -1 : 0;
}


void free_grid(struct grid *g) {
	free(g->cells);
}


static inline unsigned int mod(int a, int b) {
	/* Will only enter one of the loops, and usually loop only once so the
	  whiles are basically ifs */
	while (a < 0) {
		a += b;
	}
	while (a > b) {
		a -= b;
	}
	return a;
}

static enum cell_state _get_cell_walls(const struct grid *g, int x, int y) {
	unsigned int i = (unsigned) x;
	unsigned int j = (unsigned) y;
	return (i < g->w && j < g->h) ? GET_BIT(g->cells, g->w * j + i) : DEAD;
}

static enum cell_state _get_cell_wrap(const struct grid *g, int x, int y) {
	unsigned int i = mod(x, g->w), j = mod(y, g->h);
	return GET_BIT(g->cells, g->w * j + i);
}

enum cell_state get_grid_cell(const struct grid *g, int i, int j) {
	return (g->wrap ? _get_cell_wrap : _get_cell_walls)(g, i, j);
}


enum cell_state toggle_cell(struct grid *g, unsigned int x, unsigned int y) {
	if (x < g->w && y < g->h) {
		TOGGLE_BIT(g->cells, g->w * y + x);
		return GET_BIT(g->cells, g->w * y + x);
	}
	return false;
}


static inline bool _will_be_born(unsigned int n, const char *r) {
	char k = (char) ('0' + n);
	r = strchr(r, 'B') + 1;
	while (*r != '\0' && (*r != '/' && *r != 'S') && *r != k) {
		r++;
	}
	return *r == k;
}

static inline bool _will_survive(unsigned int n, const char *r) {
	char k = (char) ('0' + n);
	r = strchr(r, 'S') + 1;
	while (*r != '\0' && *r != k) {
		r++;
	}
	return *r == k;
}

static void _update_cell(struct grid *g, size_t row_offset,
                         const char *row_buffer, size_t cell_offset,
                         unsigned int neighbors) {
	bool (*will_be_alive)(unsigned int, const char*);
	if (GET_BIT(row_buffer, g->w + cell_offset)) {
		will_be_alive = _will_survive;
	} else {
		will_be_alive = _will_be_born;
	}
	SET_BIT(g->cells, row_offset + cell_offset,
	        will_be_alive(neighbors, g->rule));
}

static void _update_row(struct grid *g, size_t row_offset,
                        const char *row_buffer, const char *btm_row,
                        size_t btm_row_offset) {
	unsigned int neighbors = 0;
	neighbors = GET_BIT(row_buffer, 0)
	          + GET_BIT(row_buffer, 1)
	          + GET_BIT(row_buffer, g->w + 1)
	          + GET_BIT(btm_row, btm_row_offset)
	          + GET_BIT(btm_row, btm_row_offset + 1);
	if (g->wrap) {
		neighbors += GET_BIT(row_buffer, g->w - 1)
		           + GET_BIT(row_buffer, g->w * 2 - 1)
		           + GET_BIT(btm_row, btm_row_offset + g->w - 1);
	}
	_update_cell(g, row_offset, row_buffer, 0, neighbors);
	for (size_t i = 1; i < g->w - 1; ++i) {
		neighbors = GET_BIT(row_buffer, i - 1)
		          + GET_BIT(row_buffer, i)
		          + GET_BIT(row_buffer, i + 1)
		          + GET_BIT(row_buffer, g->w + i - 1)
		          + GET_BIT(row_buffer, g->w + i + 1)
		          + GET_BIT(btm_row, btm_row_offset + i - 1)
		          + GET_BIT(btm_row, btm_row_offset + i)
		          + GET_BIT(btm_row, btm_row_offset + i + 1);
		_update_cell(g, row_offset, row_buffer, i, neighbors);
	}
	neighbors = GET_BIT(row_buffer, g->w - 2)
	          + GET_BIT(row_buffer, g->w - 1)
	          + GET_BIT(row_buffer, g->w * 2 - 2)
	          + GET_BIT(btm_row, btm_row_offset + g->w - 2)
	          + GET_BIT(btm_row, btm_row_offset + g->w - 1);
	if (g->wrap) {
		neighbors += GET_BIT(row_buffer, 0)
		           + GET_BIT(row_buffer, g->w)
		           + GET_BIT(btm_row, btm_row_offset);
	}
	_update_cell(g, row_offset, row_buffer, g->w - 1, neighbors);
}

int update_grid(struct grid *g) {
	/* The most memory-efficient way of updating the grid is to use a buffer
	   of 3 rows to update the state of each row, where one row is the one being
	   updated and the other is a backup of the adjacent row that was updated
	   just before. The third row of the buffer is only used on the last row of
	   the grid.
	   This method also works on columns, however rows are usually wider than
	   columns are high so going with the rows mitigates time complexity (the
	   number of passes) by allowing a higher memory consumption (by allocating
	   a bigger buffer). */
	char *cells_buffer = calloc(NUM_OCTETS(3 * g->w), 1);
	if (cells_buffer == NULL)
		return -__LINE__;
	/* First row */
	if (g->wrap) {
		copy_bits(g->cells, (g->h - 1) * g->w, cells_buffer, 0, g->w);
		/* Save the first row's previous state for the last grid row */
		copy_bits(g->cells, 0, cells_buffer, g->w * 2, g->w);
	} /* otherwise, buffer already cleared */
	copy_bits(g->cells, 0, cells_buffer, g->w, g->w);
	_update_row(g, 0, cells_buffer, g->cells, g->w);

	/* Middle rows */
	for (size_t row = g->w; row < (g->h - 1) * g->w; row += g->w) {
		/* Move second buffer row to first; blit current grid row to second
		   buffer row then perform update on grid row */
		copy_bits(cells_buffer, g->w, cells_buffer, 0, g->w);
		copy_bits(g->cells, row, cells_buffer, g->w, g->w);
		_update_row(g, row, cells_buffer, g->cells, row + g->w);
	}

	/* Last row */
	copy_bits(cells_buffer, g->w, cells_buffer, 0, g->w);
	copy_bits(g->cells, (g->h - 1) * g->w, cells_buffer, g->w, g->w);
	_update_row(g, (g->h - 1) * g->w, cells_buffer, cells_buffer, g->w * 2);

	free(cells_buffer);
	return 0;
}


void clear_grid(struct grid *g) {
	memset(g->cells, false, NUM_OCTETS(g->w * g->h));
}
