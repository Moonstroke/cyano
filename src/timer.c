#include "timer.h"

#include <SDL2/SDL_timer.h>


void startTimer(struct timer *t) {
	t->ticks = SDL_GetTicks();
}

extern void resetTimer(struct timer *t);

unsigned int getDelta(const struct timer *t) {
	return SDL_GetTicks() - t->ticks;
}

extern unsigned int getRemainingTime(const struct timer *t);
