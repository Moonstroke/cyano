#include "board.h"


#include <stdlib.h>
#include <string.h>



static bool *getCellLimits(const Board*, int, int);
static bool *getCellWrap(const Board*, int, int);

bool initBoard(Board *const b, const unsigned int w, const unsigned int h, const bool wrap) {
	b->w = w;
	b->h = h;

	bool *const cells = calloc(w * h, sizeof(bool));
	b->cells = cells;
	b->getCell = wrap ? &getCellWrap : &getCellLimits;
	return cells != NULL;
}

void freeBoard(Board *b) {
	free(b->cells);
}


static inline unsigned int mod(int a, int b) {
	const int r = a % b;
	return r < 0 ? r + b : r;
}

static bool *getCellLimits(const Board *const b, const int x, const int y) {
	const unsigned int i = (unsigned)x, j = (unsigned)y;
	return (i < b->w && j < b->h) ? &b->cells[b->w * j + i] : NULL;
}
static bool *getCellWrap(const Board *const b, const int x, const int y) {
	const unsigned int i = mod(x, b->w), j = mod(y, b->h);
	return &b->cells[b->w * j + i];
}

bool toggleCell(Board *const b, const unsigned int x, const unsigned int y) {
	bool *const cell = b->getCell(b, x, y);
	if(cell != NULL) {
		*cell = !*cell;
		return true;
	}
	return false;
}

const char *getRules(const Board *const b) {
	return b->rules;
}

void setRules(Board *const b, const char *const r) {
	b->rules = r;
}

static inline bool willBeBorn(const unsigned int n, const char *r) {
	const char k = '0' + n;
	r = strchr(r, 'B') + 1;
	while(*r != '\0' && (*r != '/' && *r != 'S') && *r != k) {
		r++;
	}
	return *r == k;
}

static inline bool willSurvive(const unsigned int n, const char *r) {
	const char k = '0' + n;
	r = strchr(r, 'S') + 1;
	while(*r != '\0' && *r != k) {
		r++;
	}
	return *r == k;
}

static void updateRow(Board *b, const bool *prevRowBuffer,
                      const bool *btmPrevRow, size_t rowOffset, bool wraps) {
	bool *row = &b->cells[rowOffset];
	const bool *topPrevRow = prevRowBuffer, *prevRow = &prevRowBuffer[b->w];
	unsigned int neighbors = 0;
	neighbors = topPrevRow[0] + topPrevRow[1] + prevRow[1] + btmPrevRow[0]
	                          + btmPrevRow[1];
	if(wraps)
		neighbors += topPrevRow[b->w - 1] + prevRow[b->w - 1]
		                                  + btmPrevRow[b->w - 1];
	if(prevRow[0])
		row[0] = willSurvive(neighbors, b->rules);
	else
		row[0] = willBeBorn(neighbors, b->rules);
	for(size_t i = 1; i < b->w - 1; ++i) {
		neighbors = topPrevRow[i - 1] + topPrevRow[i] + topPrevRow[i + 1]
		                              + prevRow[i - 1] + prevRow[i + 1]
		                              + btmPrevRow[i - 1] + btmPrevRow[i]
		                              + btmPrevRow[i + 1];
		if(prevRow[i])
			row[i] = willSurvive(neighbors, b->rules);
		else
			row[i] = willBeBorn(neighbors, b->rules);
	}
	neighbors = topPrevRow[b->w - 2] + topPrevRow[b->w - 1] + prevRow[b->w - 2]
	                                 + btmPrevRow[b->w - 2]
	                                 + btmPrevRow[b->w - 1];
	if(wraps)
		neighbors += topPrevRow[0] + prevRow[0] + btmPrevRow[0];
	if(prevRow[b->w - 1])
		row[b->w - 1] = willSurvive(neighbors, b->rules);
	else
		row[b->w - 1] = willBeBorn(neighbors, b->rules);
}

bool updateBoard(Board *b) {
	/* The most memory-efficient way of updating the board is to use a buffer
	   of 3 rows to update the state of each row, where one row is the one being
	   updated and the other is a backup of the adjacent row that was updated
	   just before. The third row of the buffer is only used on the last row of
	   the board.
	   This method also works on columns, however rows are usually bigger than
	   columns so going with the rows is an immediate compromise between memory
	   consumption and time complexity. */
	bool *cellsBuffer = calloc(3 * b->w, sizeof(bool));
	if(cellsBuffer == NULL)
		return false;
	bool boardWraps = b->getCell == getCellWrap; /* Hacky way of checking */
	/* First row */
	if(boardWraps) {
		memcpy(cellsBuffer, &b->cells[(b->h - 1) * b->w], b->w);
		/* Save the first row's previous state for the last board row */
		memcpy(&cellsBuffer[b->w * 2], b->cells, b->w);
	} /* otherwise, buffer already cleared */
	memcpy(&cellsBuffer[b->w], b->cells, b->w);
	updateRow(b, cellsBuffer, &b->cells[b->w], 0, boardWraps);

	/* Middle rows */
	for(size_t row = b->w; row < (b->h - 1) * b->w; row += b->w) {
		/* Move second buffer row to first; blit current board row to second
		   buffer row then perform update on board row */
		memcpy(cellsBuffer, &cellsBuffer[b->w], b->w); // memcpy safe because no
		                                               // overlap
		memcpy(&cellsBuffer[b->w], &b->cells[row], b->w);
		updateRow(b, cellsBuffer, &b->cells[row + b->w], row, boardWraps);
	}

	/* Last row */
	memcpy(cellsBuffer, &cellsBuffer[b->w], b->w);
	memcpy(&cellsBuffer[b->w], &b->cells[(b->h - 1) * b->w], b->w);
	updateRow(b, cellsBuffer, &cellsBuffer[b->w * 2], (b->h - 1) * b->w,
	          boardWraps);

	free(cellsBuffer);
	return true;
}

void clearBoard(Board *const b) {
	memset(b->cells, false, b->w * b->h);
}
