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
struct timer {
	double delay; /**< The time to wait between two ticks */
	unsigned int ticks; /**< The number of ticks elapsed */
};


/**
 * \brief Start the timer.
 *
 * \param[in,out] timer The timer to start
 */
void startTimer(struct timer *timer);


/**
 * \brief Reset the timer to \c 0 and stops it.
 *
 * \param[out] timer The timer to reset
 */
inline void resetTimer(struct timer *timer) {
	timer->ticks = 0;
}


/**
 * \brief Retrieve the difference between the start of the timer and \e now.
 *
 * \param[in] timer The timer
 *
 * \return The elapsed time, in \c ms, since the timer started
 */
unsigned int getDelta(const struct timer *timer);


/**
 * \brief Calculate the remaining time to wait before the next refresh.
 *
 * \param[in] timer The timer
 *
 * \return The remaining milliseconds before the next scheduled window update
 */
inline unsigned int getRemainingTime(const struct timer *timer) {
	int time = ((unsigned int)timer->delay) - getDelta(timer);
	return time < 0 ? 0 : time;
}


#endif /* TIMER_H */
