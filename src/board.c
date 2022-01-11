#include "board.h"


#include <stdlib.h>
#include <string.h>

#include "bits.h"



int initBoard(struct board *b, unsigned int w, unsigned int h, bool wrap) {
	b->w = w;
	b->h = h;

	char *cells = calloc(NUM_OCTETS(w * h), 1);
	b->cells = cells;
	b->wrap = wrap;
	return cells == NULL ? -1 : 0;
}

void freeBoard(struct board *b) {
	free(b->cells);
}


static inline unsigned int mod(int a, int b) {
	int r = a % b;
	return r < 0 ? r + b : r;
}

static bool getCellLimits(const struct board *b, int x, int y) {
	unsigned int i = (unsigned)x, j = (unsigned)y;
	return (i < b->w && j < b->h) ? GET_BIT(b->cells, b->w * j + i) : false;
}
static bool getCellWrap(const struct board *b, int x, int y) {
	unsigned int i = mod(x, b->w), j = mod(y, b->h);
	return GET_BIT(b->cells, b->w * j + i);
}

bool getBoardCell(struct board *b, int i, int j) {
	return (b->wrap ? getCellWrap : getCellLimits)(b, i, j);
}

bool toggleCell(struct board *b, unsigned int x, unsigned int y) {
	if(x < b->w && y < b->h) {
		TOGGLE_BIT(b->cells, b->w * y + x);
		return GET_BIT(b->cells, b->w * y + x);
	}
	return false;
}

const char *getRules(const struct board *b) {
	return b->rules;
}

void setRules(struct board *b, const char *r) {
	b->rules = r;
}

static inline bool willBeBorn(unsigned int n, const char *r) {
	char k = '0' + n;
	r = strchr(r, 'B') + 1;
	while(*r != '\0' && (*r != '/' && *r != 'S') && *r != k) {
		r++;
	}
	return *r == k;
}

static inline bool willSurvive(unsigned int n, const char *r) {
	char k = '0' + n;
	r = strchr(r, 'S') + 1;
	while(*r != '\0' && *r != k) {
		r++;
	}
	return *r == k;
}

static void updateRow(struct board *b, const bool *prevRowBuffer,
                      const bool *btmPrevRow, size_t rowOffset) { // TODO update w/ copyBits
	bool *row = &b->cells[rowOffset];
	const bool *topPrevRow = prevRowBuffer, *prevRow = &prevRowBuffer[b->w];
	unsigned int neighbors = 0;
	neighbors = topPrevRow[0] + topPrevRow[1] + prevRow[1] + btmPrevRow[0]
	                          + btmPrevRow[1];
	if(b->wrap)
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
	if(b->wrap)
		neighbors += topPrevRow[0] + prevRow[0] + btmPrevRow[0];
	if(prevRow[b->w - 1])
		row[b->w - 1] = willSurvive(neighbors, b->rules);
	else
		row[b->w - 1] = willBeBorn(neighbors, b->rules);
}

int updateBoard(struct board *b) {
	/* The most memory-efficient way of updating the board is to use a buffer
	   of 3 rows to update the state of each row, where one row is the one being
	   updated and the other is a backup of the adjacent row that was updated
	   just before. The third row of the buffer is only used on the last row of
	   the board.
	   This method also works on columns, however rows are usually bigger than
	   columns so going with the rows is an immediate compromise between memory
	   consumption and time complexity. */
	char *cellsBuffer = calloc(NUM_OCTETS(3 * b->w), 1);
	if(cellsBuffer == NULL)
		return -1;
	/* First row */
	if(b->wrap) {
		copyBits(b->cells, (b->h - 1) * b->w, cellsBuffer, 0, b->w);
		/* Save the first row's previous state for the last board row */
		copyBits(b->cells, 0, cellsBuffer, b->w * 2, b->w);
	} /* otherwise, buffer already cleared */
	copyBits(b->cells, b->w, cellsBuffer, b->w, b->w);
	updateRow(b, cellsBuffer, &b->cells[b->w], 0);

	/* Middle rows */
	for(size_t row = b->w; row < (b->h - 1) * b->w; row += b->w) {
		/* Move second buffer row to first; blit current board row to second
		   buffer row then perform update on board row */
		copyBits(cellsBuffer, b->w, cellsBuffer, 0, b->w);
		copyBits(b->cells, row, cellsBuffer, b->w, b->w);
		updateRow(b, cellsBuffer, &b->cells[row + b->w], row);
	}

	/* Last row */
	copyBits(cellsBuffer, b->w, cellsBuffer, 0, b->w);
	copyBits(b->cells, (b->h - 1) * b->w, cellsBuffer, b->w, b->w);
	updateRow(b, cellsBuffer, &cellsBuffer[b->w * 2], (b->h - 1) * b->w);

	free(cellsBuffer);
	return 0;
}

void clearBoard(struct board *b) {
	memset(b->cells, false, NUM_OCTETS(b->w * b->h));
}
