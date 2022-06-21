#include "timer.h"

#include <SDL2/SDL_timer.h>



void start_timer(struct timer *timer) {
	timer->ticks = SDL_GetTicks();
}


extern void reset_timer(struct timer*);


unsigned int get_timer_delta(const struct timer *timer) {
	return SDL_GetTicks() - timer->ticks;
}


extern unsigned int get_remaining_time(const struct timer*);
