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
	return i < b->w && j < b->h ? b->cells + (b->w * j + i) : NULL;
}
static bool *getCellWrap(const Board *const b, const int x, const int y) {
	const unsigned int i = mod(x, b->w), j = mod(y, b->h);
	return b->cells + (b->w * j + i);
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

static inline unsigned int neighbors(const Board *const b, unsigned int x, unsigned int y) {
	unsigned int n = 0, i;
	bool *cell;

	const int coords[][2] = {
	        {x - 1, y - 1},
	        {x    , y - 1},
	        {x + 1, y - 1},
	        {x - 1, y    },
	        {x + 1, y    },
	        {x - 1, y + 1},
	        {x    , y + 1},
	        {x + 1, y + 1}
	    };
	for(i = 0; i < 8; ++i) {
		cell = b->getCell(b, coords[i][0], coords[i][1]);
		if(cell != NULL && *cell == true) {
			n += 1;
		}
	}

	return n;
}

bool updateBoard(Board *const b) {
	const unsigned int w = b->w, h = b->h;
	unsigned int i, j;
	bool *cells;

	cells = malloc(w * h * sizeof(bool));
	if(cells == NULL)
		return false;

	for(j = 0; j < h; ++j) {
		for(i = 0; i < w; ++i) {
			if(*b->getCell(b, i, j))
				cells[w * j + i] = willSurvive(neighbors(b, i, j), b->rules);
			else
				cells[w * j + i] = willBeBorn(neighbors(b, i, j), b->rules);
		}
	}
	free(b->cells);
	b->cells = cells;
	return true;
}

void clearBoard(Board *const b) {
	const unsigned int w = b->w, h = b->h;
	unsigned int i, j;
	for(j = 0; j < h; ++j)
		for(i = 0; i < w; ++i)
			*b->getCell(b, i, j) = false;
}
