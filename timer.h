#ifndef TIMER_H
#define TIMER_H

typedef struct {
	double delay;
	unsigned int ticks;
	unsigned char padding[4];
} Timer;

void startTimer(Timer *self);

inline void resetTimer(Timer *const self) {
	self->ticks = 0;
}

inline void setDelay(Timer *const self, const double delay) {
	self->delay = delay;
}

inline unsigned int getTicks(const Timer *const self) {
	return self->ticks;
}

unsigned int getDelta(const Timer *self);

inline unsigned int getRemainingTime(const Timer *self) {
	const int time = ((unsigned int)self->delay) - getDelta(self);
	return time < 0 ? 0 : time;
}

#endif // TIMER_H
