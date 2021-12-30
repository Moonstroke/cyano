#include "timer.h"

#include <SDL2/SDL_timer.h>


void startTimer(Timer *t) {
	t->ticks = SDL_GetTicks();
}

extern void resetTimer(Timer *t);
extern void setDelay(Timer *t, double d);

extern unsigned int getTicks(const Timer *t);

unsigned int getDelta(const Timer *t) {
	return SDL_GetTicks() - t->ticks;
}

extern unsigned int getRemainingTime(const Timer *t);
