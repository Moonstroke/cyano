#include "grid.h"


#include <stdlib.h> /* for calloc, NULL, free */
#include <string.h> /* for strchr, memset */

#include "bits.h"



int initGrid(struct grid *g, unsigned int w, unsigned int h, bool wrap) {
	g->w = w;
	g->h = h;

	char *cells = calloc(NUM_OCTETS(w * h), 1);
	g->cells = cells;
	g->wrap = wrap;
	return cells == NULL ? -1 : 0;
}


void freeGrid(struct grid *g) {
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

static bool _getCellLimits(const struct grid *g, int x, int y) {
	unsigned int i = (unsigned)x, j = (unsigned)y;
	return (i < g->w && j < g->h) ? GET_BIT(g->cells, g->w * j + i) : false;
}

static bool _getCellWrap(const struct grid *g, int x, int y) {
	unsigned int i = mod(x, g->w), j = mod(y, g->h);
	return GET_BIT(g->cells, g->w * j + i);
}

bool getGridCell(const struct grid *g, int i, int j) {
	return (g->wrap ? _getCellWrap : _getCellLimits)(g, i, j);
}


bool toggleCell(struct grid *g, unsigned int x, unsigned int y) {
	if (x < g->w && y < g->h) {
		TOGGLE_BIT(g->cells, g->w * y + x);
		return GET_BIT(g->cells, g->w * y + x);
	}
	return false;
}


static inline bool _willBeBorn(unsigned int n, const char *r) {
	char k = '0' + n;
	r = strchr(r, 'B') + 1;
	while (*r != '\0' && (*r != '/' && *r != 'S') && *r != k) {
		r++;
	}
	return *r == k;
}

static inline bool _willSurvive(unsigned int n, const char *r) {
	char k = '0' + n;
	r = strchr(r, 'S') + 1;
	while (*r != '\0' && *r != k) {
		r++;
	}
	return *r == k;
}

static void _updateCell(struct grid *g, size_t rowOffset, const char *rowBuffer,
                        size_t cellOffset, unsigned int neighbors) {
	bool (*willBeAlive)(unsigned int, const char*);
	if (GET_BIT(rowBuffer, g->w + cellOffset)) {
		willBeAlive = _willSurvive;
	} else {
		willBeAlive = _willBeBorn;
	}
	SET_BIT(g->cells, rowOffset + cellOffset, willBeAlive(neighbors, g->rule));
}

static void _updateRow(struct grid *g, size_t rowOffset, const char *rowBuffer,
                       const char *btmRow, size_t btmRowOffset) {
	unsigned int neighbors = 0;
	neighbors = GET_BIT(rowBuffer, 0)
	          + GET_BIT(rowBuffer, 1)
	          + GET_BIT(rowBuffer, g->w + 1)
	          + GET_BIT(btmRow, btmRowOffset)
	          + GET_BIT(btmRow, btmRowOffset + 1);
	if (g->wrap) {
		neighbors += GET_BIT(rowBuffer, g->w - 1)
		           + GET_BIT(rowBuffer, g->w * 2 - 1)
		           + GET_BIT(btmRow, btmRowOffset + g->w - 1);
	}
	_updateCell(g, rowOffset, rowBuffer, 0, neighbors);
	for (size_t i = 1; i < g->w - 1; ++i) {
		neighbors = GET_BIT(rowBuffer, i - 1)
		          + GET_BIT(rowBuffer, i)
		          + GET_BIT(rowBuffer, i + 1)
		          + GET_BIT(rowBuffer, g->w + i - 1)
		          + GET_BIT(rowBuffer, g->w + i + 1)
		          + GET_BIT(btmRow, btmRowOffset + i - 1)
		          + GET_BIT(btmRow, btmRowOffset + i)
		          + GET_BIT(btmRow, btmRowOffset + i + 1);
		_updateCell(g, rowOffset, rowBuffer, i, neighbors);
	}
	neighbors = GET_BIT(rowBuffer, g->w - 2)
	          + GET_BIT(rowBuffer, g->w - 1)
	          + GET_BIT(rowBuffer, g->w * 2 - 2)
	          + GET_BIT(btmRow, btmRowOffset + g->w - 2)
	          + GET_BIT(btmRow, btmRowOffset + g->w - 1);
	if (g->wrap) {
		neighbors += GET_BIT(rowBuffer, 0)
		           + GET_BIT(rowBuffer, g->w)
		           + GET_BIT(btmRow, btmRowOffset);
	}
	_updateCell(g, rowOffset, rowBuffer, g->w - 1, neighbors);
}

int updateGrid(struct grid *g) {
	/* The most memory-efficient way of updating the grid is to use a buffer
	   of 3 rows to update the state of each row, where one row is the one being
	   updated and the other is a backup of the adjacent row that was updated
	   just before. The third row of the buffer is only used on the last row of
	   the grid.
	   This method also works on columns, however rows are usually wider than
	   columns are high so going with the rows mitigates time complexity (the
	   number of passes) by allowing a higher memory consumption (by allocating
	   a bigger buffer). */
	char *cellsBuffer = calloc(NUM_OCTETS(3 * g->w), 1);
	if (cellsBuffer == NULL)
		return -1;
	/* First row */
	if (g->wrap) {
		copyBits(g->cells, (g->h - 1) * g->w, cellsBuffer, 0, g->w);
		/* Save the first row's previous state for the last grid row */
		copyBits(g->cells, 0, cellsBuffer, g->w * 2, g->w);
	} /* otherwise, buffer already cleared */
	copyBits(g->cells, 0, cellsBuffer, g->w, g->w);
	_updateRow(g, 0, cellsBuffer, g->cells, g->w);

	/* Middle rows */
	for (size_t row = g->w; row < (g->h - 1) * g->w; row += g->w) {
		/* Move second buffer row to first; blit current grid row to second
		   buffer row then perform update on grid row */
		copyBits(cellsBuffer, g->w, cellsBuffer, 0, g->w);
		copyBits(g->cells, row, cellsBuffer, g->w, g->w);
		_updateRow(g, row, cellsBuffer, g->cells, row + g->w);
	}

	/* Last row */
	copyBits(cellsBuffer, g->w, cellsBuffer, 0, g->w);
	copyBits(g->cells, (g->h - 1) * g->w, cellsBuffer, g->w, g->w);
	_updateRow(g, (g->h - 1) * g->w, cellsBuffer, cellsBuffer, g->w * 2);

	free(cellsBuffer);
	return 0;
}


void clearGrid(struct grid *g) {
	memset(g->cells, false, NUM_OCTETS(g->w * g->h));
}
