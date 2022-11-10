#include "grid.h"


#include <stdlib.h> /* for calloc, NULL, free */
#include <string.h> /* for strchr, memset */

#include "bits.h"
#include "mathutils.h" /* for pos_mod */
#include "utils.h" /* for CHECK_NULL */



int init_grid(struct grid *grid, unsigned int width, unsigned int height,
              bool wrap) {
	grid->width = width;
	grid->height = height;

	char *cells = calloc(num_octets(width * height), 1);
	grid->cells = cells;
	grid->wrap = wrap;
	memset(grid->rule, 0, sizeof grid->rule);
	return cells == NULL ? -1 : 0;
}


void free_grid(struct grid *grid) {
	free(grid->cells);
}


static enum cell_state _get_cell_walls(const struct grid *grid, int row,
                                       int col) {
	if (0 <= row && (unsigned) row < grid->width
	    && 0 <= col && (unsigned) col < grid->height) {
		return get_bit(grid->cells, grid->width * row + col);
	}
	return DEAD;
}

static enum cell_state _get_cell_wrap(const struct grid *grid, int row,
                                      int col) {
	unsigned int row_wrapped = pos_mod(row, grid->height);
	unsigned int col_wrapped = pos_mod(col, grid->width);
	return get_bit(grid->cells, grid->width * row_wrapped + col_wrapped);
}

enum cell_state get_grid_cell(const struct grid *grid, int row, int col) {
	return (grid->wrap ? &_get_cell_wrap : &_get_cell_walls)(grid, row, col);
}


enum cell_state _toggle_cell_wrap(struct grid *grid, int row, int col) {
	unsigned int row_wrapped = pos_mod(row, grid->height);
	unsigned int col_wrapped = pos_mod(col, grid->width);
	toggle_bit(grid->cells, grid->width * row_wrapped + col_wrapped);
	return get_bit(grid->cells, grid->width * row + col);
}

enum cell_state _toggle_cell_walls(struct grid *grid, int row, int col) {
	if (0 <= row && (unsigned) row < grid->height
	    && 0 <= col && (unsigned) col < grid->width) {
		unsigned int cell_bit_index = grid->width * row + col;
		toggle_bit(grid->cells, cell_bit_index);
		return get_bit(grid->cells, cell_bit_index);
	}
	return DEAD;
}

enum cell_state toggle_cell(struct grid *grid, int row, int col) {
	return (grid->wrap ? &_toggle_cell_wrap : &_toggle_cell_walls)(grid, row,
	                                                               col);
}


static inline bool _will_be_born(unsigned int num_neighbors,
                                 const char *rulestring) {
	char num_neighbors_char = (char) ('0' + num_neighbors);
	rulestring = strchr(rulestring, 'B') + 1;
	while (*rulestring != '\0' && *rulestring != '/' && *rulestring != 'S'
	                           && *rulestring != num_neighbors_char) {
		++rulestring;
	}
	return *rulestring == num_neighbors_char;
}

static inline bool _will_survive(unsigned int num_neighbors,
                                 const char *rulestring) {
	char num_neighbors_char = (char) ('0' + num_neighbors);
	rulestring = strchr(rulestring, 'S') + 1;
	while (*rulestring != '\0' && *rulestring != num_neighbors_char) {
		++rulestring;
	}
	return *rulestring == num_neighbors_char;
}

static void _update_cell(struct grid *grid, size_t row_offset,
                         const char *row_buffer, size_t cell_offset,
                         unsigned int neighbors) {
	bool (*will_be_alive)(unsigned int, const char*);
	if (get_bit(row_buffer, grid->width + cell_offset) == 0) {
		will_be_alive = _will_be_born;
	} else {
		will_be_alive = _will_survive;
	}
	set_bit(grid->cells, row_offset + cell_offset,
	        will_be_alive(neighbors, grid->rule));
}

static void _update_row(struct grid *grid, size_t row_offset,
                        const char *row_buffer, const char *btm_row,
                        size_t btm_row_offset) {
	unsigned int neighbors = 0;
	neighbors = get_bit(row_buffer, 0)
	          + get_bit(row_buffer, 1)
	          + get_bit(row_buffer, grid->width + 1)
	          + get_bit(btm_row, btm_row_offset)
	          + get_bit(btm_row, btm_row_offset + 1);
	if (grid->wrap) {
		neighbors += get_bit(row_buffer, grid->width - 1)
		           + get_bit(row_buffer, grid->width * 2 - 1)
		           + get_bit(btm_row, btm_row_offset + grid->width - 1);
	}
	_update_cell(grid, row_offset, row_buffer, 0, neighbors);
	for (size_t col = 1; col < grid->width - 1; ++col) {
		neighbors = get_bit(row_buffer, col - 1)
		          + get_bit(row_buffer, col)
		          + get_bit(row_buffer, col + 1)
		          + get_bit(row_buffer, grid->width + col - 1)
		          + get_bit(row_buffer, grid->width + col + 1)
		          + get_bit(btm_row, btm_row_offset + col - 1)
		          + get_bit(btm_row, btm_row_offset + col)
		          + get_bit(btm_row, btm_row_offset + col + 1);
		_update_cell(grid, row_offset, row_buffer, col, neighbors);
	}
	neighbors = get_bit(row_buffer, grid->width - 2)
	          + get_bit(row_buffer, grid->width - 1)
	          + get_bit(row_buffer, grid->width * 2 - 2)
	          + get_bit(btm_row, btm_row_offset + grid->width - 2)
	          + get_bit(btm_row, btm_row_offset + grid->width - 1);
	if (grid->wrap) {
		neighbors += get_bit(row_buffer, 0)
		           + get_bit(row_buffer, grid->width)
		           + get_bit(btm_row, btm_row_offset);
	}
	_update_cell(grid, row_offset, row_buffer, grid->width - 1, neighbors);
}

int update_grid(struct grid *grid) {
	/* The most memory-efficient way of updating the grid is to use a buffer
	   of 3 rows to update the state of each row, where one row is the one being
	   updated and the other is a backup of the adjacent row that was updated
	   just before. The third row of the buffer is only used on the last row of
	   the grid.
	   This method also works on columns, however rows are usually wider than
	   columns are high so going with the rows mitigates time complexity (the
	   number of passes) by allowing a higher memory consumption (by allocating
	   a bigger buffer). */
	char *cells_buffer = calloc(num_octets(3 * grid->width), 1);
	CHECK_NULL(cells_buffer);
	/* First row */
	if (grid->wrap) {
		copy_bits(grid->cells, (grid->height - 1) * grid->width, cells_buffer,
		          0, grid->width);
		/* Save the first row's previous state for the last grid row */
		copy_bits(grid->cells, 0, cells_buffer, grid->width * 2, grid->width);
	} /* otherwise, buffer already cleared */
	copy_bits(grid->cells, 0, cells_buffer, grid->width, grid->width);
	_update_row(grid, 0, cells_buffer, grid->cells, grid->width);

	/* Middle rows */
	for (size_t row = grid->width; row < (grid->height - 1) * grid->width;
	     row += grid->width) {
		/* Move second buffer row to first; blit current grid row to second
		   buffer row then perform update on grid row */
		copy_bits(cells_buffer, grid->width, cells_buffer, 0, grid->width);
		copy_bits(grid->cells, row, cells_buffer, grid->width, grid->width);
		_update_row(grid, row, cells_buffer, grid->cells, row + grid->width);
	}

	/* Last row */
	copy_bits(cells_buffer, grid->width, cells_buffer, 0, grid->width);
	copy_bits(grid->cells, (grid->height - 1) * grid->width, cells_buffer,
	          grid->width, grid->width);
	_update_row(grid, (grid->height - 1) * grid->width, cells_buffer, cells_buffer,
	            grid->width * 2);

	free(cells_buffer);
	return 0;
}


void clear_grid(struct grid *grid) {
	memset(grid->cells, DEAD, num_octets(grid->width * grid->height));
}
