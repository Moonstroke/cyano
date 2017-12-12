#include "life.h"


#include <stdlib.h>

#include "log.h"



bool initBoard(Board *const b, const unsigned int w, const unsigned int h) {
	b->w = w;
	b->h = h;

	bool *const cells = calloc(w * h, sizeof(bool));
	b->cells = cells;
	return cells != NULL;
}

void freeBoard(Board *b) {
	free(b->cells);
}


bool getCell(const Board *const b, const unsigned int x, const unsigned int y) {
	return x < b->w && y < b->h && b->cells[b->w * y + x];
}

bool toggleCell(Board *const b, const unsigned int x, const unsigned int y) {
	if(x < b->w && y < b->h) {
		b->cells[b->w * y + x] = !b->cells[b->w * y + x];
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
	if(getCell(b, x - 1, y - 1)) n += 1;
	if(getCell(b, x    , y - 1)) n += 1;
	if(getCell(b, x + 1, y - 1)) n += 1;
	if(getCell(b, x - 1, y    )) n += 1;
	if(getCell(b, x + 1, y    )) n += 1;
	if(getCell(b, x - 1, y + 1)) n += 1;
	if(getCell(b, x    , y + 1)) n += 1;
	if(getCell(b, x + 1, y + 1)) n += 1;
	return n;
}

bool nextGen(Board *const b) {
	const unsigned int w = b->w, h = b->h;
	bool *cells = malloc(w * h * sizeof(bool));
	if(cells == NULL)
		return false;

	unsigned int i, j;
	for(j = 0; j < h; ++j) {
		for(i = 0; i < w; ++i) {
			const unsigned int n = neighbors(b, i, j);
			// FIXME use rules
			cells[w * j + i] = (n == 3 || (n == 2 && getCell(b, i, j)));
		}
	}
	free(b->cells);
	b->cells = cells;
	return true;
}

void clear(Board *const b) {
	const unsigned int w = b->w, h = b->h;
	unsigned int i, j;
	for(j = 0; j < h; ++j)
		for(i = 0; i < w; ++i)
			b->cells[w * j + i] = false;
}
