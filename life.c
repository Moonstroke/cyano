#include "life.h"


#include <stdlib.h>

#include "log.h"



bool initBoard(Board *const b, const unsigned int w, const unsigned int h) {
	b->w = w;
	b->h = h;

	bool *const cells = calloc(w * h, sizeof(bool));
	b->cells = cells;
	b->changed = true;
	return cells != NULL;
}

void freeBoard(Board *b) {
	free(b->cells);
}


bool *getCell(const Board *const b, const unsigned int x, const unsigned int y) {
	return x < b->w && y < b->h ? b->cells + (b->w * y + x) : NULL;
}

bool toggleCell(Board *const b, const unsigned int x, const unsigned int y) {
	bool *const cell = getCell(b, x, y);
	if(cell != NULL) {
		*cell = !*cell;
		b->changed = true;
		return true;
	}
	return false;
}

Rules getRules(const Board *const b) {
	return b->rules;
}

void setRules(Board *const b, Rules r) {
	b->rules = r;
}


static inline unsigned int neighbors(const Board *const b, unsigned int x, unsigned int y) {
	unsigned int n = 0;
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

	for(unsigned int i = 0; i < 8; ++i) {
		cell = getCell(b, coords[i][0], coords[i][1]);
		if(cell != NULL && *cell == true) n += 1;
	}

	return n;
}

bool updateBoard(Board *const b) {
	const unsigned int w = b->w, h = b->h;
	bool *cells = malloc(w * h * sizeof(bool));
	if(cells == NULL)
		return false;

	unsigned int i, j;
	for(j = 0; j < h; ++j) {
		for(i = 0; i < w; ++i) {
			const unsigned int n = neighbors(b, i, j);
			bool cell = *getCell(b, i, j);
			// FIXME use rules
			cells[w * j + i] = (n == 3 || (n == 2 && cell));
		}
	}
	free(b->cells);
	b->cells = cells;
	b->changed = true;
	return true;
}

void clearBoard(Board *const b) {
	const unsigned int w = b->w, h = b->h;
	unsigned int i, j;
	for(j = 0; j < h; ++j)
		for(i = 0; i < w; ++i)
			*getCell(b, i, j) = false;
	b->changed = true;
}
