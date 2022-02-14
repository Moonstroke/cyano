#include "timer.h"

#include <SDL2/SDL_timer.h>



void startTimer(struct timer *timer) {
	timer->ticks = SDL_GetTicks();
}


extern void resetTimer(struct timer*);


unsigned int getDelta(const struct timer *timer) {
	return SDL_GetTicks() - timer->ticks;
}


extern unsigned int getRemainingTime(const struct timer*);
