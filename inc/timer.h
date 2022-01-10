/**
 * \file "timer.h"
 * \author joH1
 *
 * \version 0.1
 *
 * \brief This file defines the timer structure, used to regulate the rendering
 *        rate of the window.
 *
 */

#ifndef TIMER_H
#define TIMER_H


/**
 * \brief The structure holding the definition of a time scheduler.
 */
typedef struct {
	double delay;
	/**< The time to wait between two ticks */
	unsigned int ticks;
	/**< The number of ticks elapsed */
	unsigned char padding[4];
	/**< Unused value -- this is only here to pad the structure size */
} Timer;


/**
 * \brief Starts the timer.
 *
 * \param[in,out] self The timer to start
 */
void startTimer(Timer *self);

/**
 * \brief Resets the timer to \c 0 and stops it.
 *
 * \param[in,out] self The timer to reset
 */
inline void resetTimer(Timer *self) {
	self->ticks = 0;
}


/**
 * \brief Sets the delay of the timer (i.e. the time to wait between two
          renderings).
 *
 * \param[in,out] self  The timer
 * \param[in]     delay The delay to set
 */
inline void setDelay(Timer *self, double delay) {
	self->delay = delay;
}


/**
 * \brief Retrieves the current timing of the timer.
 *
 * \param[in] self The timer
 *
 * \return The current milliseconds since the start of the timer
 *
 * \note This value is represented as an unsigned integer. This type can last
 *       approximately \c 49 days and \c 17 hours before overflowing.
 */
inline unsigned int getTicks(const Timer *self) {
	return self->ticks;
}

/**
 * \brief Retrieve the difference between the start of the timer and \e now.
 *
 * \param[in] self The timer
 *
 * \return The elapsed time, in \c ms, since the timer started
 */
unsigned int getDelta(const Timer *self);

/**
 * \brief Calculates the remaining time to wait before the next refresh.
 *
 * \param[in] self The timer
 *
 * \return The remaining milliseconds before the next scheduled window update
 */
inline unsigned int getRemainingTime(const Timer *self) {
	int time = ((unsigned int)self->delay) - getDelta(self);
	return time < 0 ? 0 : time;
}


#endif // TIMER_H
