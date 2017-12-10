#ifndef LIFE_H
#define LIFE_H

#include <SDL2/SDL_render.h>
#include <stdbool.h>


typedef struct {
	const char *b, *s;
} Rules;

typedef struct {
	unsigned int w, h;
	bool *cells;
	Rules rules;
} Board;

bool initBoard(Board *board, unsigned int width, unsigned int height);

void freeBoard(Board *board);

bool getCell(const Board *board, unsigned int x, unsigned int y);
bool toggleCell(Board *board, unsigned int x, unsigned int y);

Rules getRules(const Board *board);
void setRules(Board *board, Rules rules);

bool nextGen(Board *board);

void renderBoard(Board *board, SDL_Renderer *renderer, unsigned int cell_dim);

#endif // LIFE_H