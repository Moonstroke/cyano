#ifndef LIFE_H
#define LIFE_H

#include <stdbool.h>


#define DEFAULT_BOARD_WIDTH  80
#define DEFAULT_BOARD_HEIGHT 60


typedef struct {
	const char *b, *s;
} Rules;

typedef struct {
	unsigned int w, h;
	bool *cells;
	Rules rules;
	bool changed;
	unsigned char padding[7];
} Board;

bool initBoard(Board *board, unsigned int width, unsigned int height);

void freeBoard(Board *board);

bool getCell(const Board *board, unsigned int x, unsigned int y);
bool toggleCell(Board *board, unsigned int x, unsigned int y);

Rules getRules(const Board *board);
void setRules(Board *board, Rules rules);

bool nextGen(Board *board);

void clear(Board *board);


#endif // LIFE_H
