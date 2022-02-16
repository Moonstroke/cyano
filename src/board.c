#include "board.h"


#include <stdlib.h> /* for calloc, NULL, free */
#include <string.h> /* for strchr, memset */

#include "bits.h"



int initBoard(struct board *b, unsigned int w, unsigned int h, bool wrap) {
	b->w = w;
	b->h = h;

	char *cells = calloc(NUM_OCTETS(w * h), 1);
	b->cells = cells;
	b->wrap = wrap;
	return cells == NULL ? -1 : 0;
}


static inline int initBoardFromRepr(struct board *board, const char *repr,
                                    bool wrap) {
	const char *itr = repr;
	for (; *itr && *itr != '\n'; ++itr);
	unsigned int width = itr - repr;
	unsigned int height = 1;
	/* Stepping width characters is faster, but assumes repr is well-formed */
	for (; *itr; itr += width) {
		if (*itr == '\n') {
			++height;
		} else {
			/* We did not fall on a line break: repr is ill-formed */
			return -2;
		}
	}
	return initBoard(board, width, height, wrap);
}

int loadBoard(struct board *board, const char *repr, bool wrap) {
	int rc = initBoardFromRepr(board, repr, wrap);
	if (rc < 0) {
		return rc;
	}

	for (size_t i = 0; repr[i]; ++i) {
		if (repr[i] == '@') {
			SET_BIT(board->cells, i, true);
		} else if (repr[i] != '.' && repr[i] != '\n') {
			return -3;
		}
	}
	return 0;
}

void freeBoard(struct board *b) {
	free(b->cells);
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

static bool getCellLimits(const struct board *b, int x, int y) {
	unsigned int i = (unsigned)x, j = (unsigned)y;
	return (i < b->w && j < b->h) ? GET_BIT(b->cells, b->w * j + i) : false;
}

static bool getCellWrap(const struct board *b, int x, int y) {
	unsigned int i = mod(x, b->w), j = mod(y, b->h);
	return GET_BIT(b->cells, b->w * j + i);
}

bool getBoardCell(const struct board *b, int i, int j) {
	return (b->wrap ? getCellWrap : getCellLimits)(b, i, j);
}


bool toggleCell(struct board *b, unsigned int x, unsigned int y) {
	if (x < b->w && y < b->h) {
		TOGGLE_BIT(b->cells, b->w * y + x);
		return GET_BIT(b->cells, b->w * y + x);
	}
	return false;
}


static inline bool willBeBorn(unsigned int n, const char *r) {
	char k = '0' + n;
	r = strchr(r, 'B') + 1;
	while (*r != '\0' && (*r != '/' && *r != 'S') && *r != k) {
		r++;
	}
	return *r == k;
}

static inline bool willSurvive(unsigned int n, const char *r) {
	char k = '0' + n;
	r = strchr(r, 'S') + 1;
	while (*r != '\0' && *r != k) {
		r++;
	}
	return *r == k;
}

static void updateCell(struct board *b, size_t rowOffset,
                       const char *rowBuffer, size_t cellOffset,
                       unsigned int neighbors) {
	bool (*willBeAlive)(unsigned int, const char*);
	if (GET_BIT(rowBuffer, b->w + cellOffset)) {
		willBeAlive = willSurvive;
	} else {
		willBeAlive = willBeBorn;
	}
	SET_BIT(b->cells, rowOffset + cellOffset, willBeAlive(neighbors, b->rules));
}

static void updateRow(struct board *b, size_t rowOffset, const char *rowBuffer,
                      const char *btmRow, size_t btmRowOffset) {
	unsigned int neighbors = 0;
	neighbors = GET_BIT(rowBuffer, 0)
	          + GET_BIT(rowBuffer, 1)
	          + GET_BIT(rowBuffer, b->w + 1)
	          + GET_BIT(btmRow, btmRowOffset)
	          + GET_BIT(btmRow, btmRowOffset + 1);
	if (b->wrap) {
		neighbors += GET_BIT(rowBuffer, b->w - 1)
		           + GET_BIT(rowBuffer, b->w * 2 - 1)
		           + GET_BIT(btmRow, btmRowOffset + b->w - 1);
	}
	updateCell(b, rowOffset, rowBuffer, 0, neighbors);
	for (size_t i = 1; i < b->w - 1; ++i) {
		neighbors = GET_BIT(rowBuffer, i - 1)
		          + GET_BIT(rowBuffer, i)
		          + GET_BIT(rowBuffer, i + 1)
		          + GET_BIT(rowBuffer, b->w + i - 1)
		          + GET_BIT(rowBuffer, b->w + i + 1)
		          + GET_BIT(btmRow, btmRowOffset + i - 1)
		          + GET_BIT(btmRow, btmRowOffset + i)
		          + GET_BIT(btmRow, btmRowOffset + i + 1);
		updateCell(b, rowOffset, rowBuffer, i, neighbors);
	}
	neighbors = GET_BIT(rowBuffer, b->w - 2)
	          + GET_BIT(rowBuffer, b->w - 1)
	          + GET_BIT(rowBuffer, b->w * 2 - 2)
	          + GET_BIT(btmRow, btmRowOffset + b->w - 2)
	          + GET_BIT(btmRow, btmRowOffset + b->w - 1);
	if (b->wrap) {
		neighbors += GET_BIT(rowBuffer, 0)
		           + GET_BIT(rowBuffer, b->w)
		           + GET_BIT(btmRow, btmRowOffset);
	}
	updateCell(b, rowOffset, rowBuffer, b->w - 1, neighbors);
}

int updateBoard(struct board *b) {
	/* The most memory-efficient way of updating the board is to use a buffer
	   of 3 rows to update the state of each row, where one row is the one being
	   updated and the other is a backup of the adjacent row that was updated
	   just before. The third row of the buffer is only used on the last row of
	   the board.
	   This method also works on columns, however rows are usually wider than
	   columns are high so going with the rows mitigates time complexity (the
	   number of passes) by allowing a higher memory consumption (by allocating
	   a bigger buffer). */
	char *cellsBuffer = calloc(NUM_OCTETS(3 * b->w), 1);
	if (cellsBuffer == NULL)
		return -1;
	/* First row */
	if (b->wrap) {
		copyBits(b->cells, (b->h - 1) * b->w, cellsBuffer, 0, b->w);
		/* Save the first row's previous state for the last board row */
		copyBits(b->cells, 0, cellsBuffer, b->w * 2, b->w);
	} /* otherwise, buffer already cleared */
	copyBits(b->cells, 0, cellsBuffer, b->w, b->w);
	updateRow(b, 0, cellsBuffer, b->cells, b->w);

	/* Middle rows */
	for (size_t row = b->w; row < (b->h - 1) * b->w; row += b->w) {
		/* Move second buffer row to first; blit current board row to second
		   buffer row then perform update on board row */
		copyBits(cellsBuffer, b->w, cellsBuffer, 0, b->w);
		copyBits(b->cells, row, cellsBuffer, b->w, b->w);
		updateRow(b, row, cellsBuffer, b->cells, row + b->w);
	}

	/* Last row */
	copyBits(cellsBuffer, b->w, cellsBuffer, 0, b->w);
	copyBits(b->cells, (b->h - 1) * b->w, cellsBuffer, b->w, b->w);
	updateRow(b, (b->h - 1) * b->w, cellsBuffer, cellsBuffer, b->w * 2);

	free(cellsBuffer);
	return 0;
}


void clearBoard(struct board *b) {
	memset(b->cells, false, NUM_OCTETS(b->w * b->h));
}
