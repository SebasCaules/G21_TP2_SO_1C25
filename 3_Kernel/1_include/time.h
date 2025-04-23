#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

/**
 * @brief Timer interrupt handler that manages timer events.
 * This function is called by the interrupt handler to update the system's tick count
 * and manage any scheduled timer events.
 */
void timer_handler();

/**
 * @brief Retrieves the number of ticks that have elapsed since the timer was started.
 * @return The number of ticks elapsed.
 */
int ticks_elapsed();

/**
 * @brief Retrieves the number of seconds that have elapsed since the timer was started.
 * @return The number of seconds elapsed.
 */
int seconds_elapsed();

/**
 * @brief Retrieves the total number of milliseconds that have elapsed since the timer was started.
 * @return The number of milliseconds elapsed.
 */
uint64_t ms_elapsed();

#endif
