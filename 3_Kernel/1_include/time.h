#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

/**
 * @brief Timer interrupt handler that manages timer events.
 * This function is called by the interrupt handler to update the system's tick count
 * and manage any scheduled timer events.
 */
uint64_t timer_handler(uint64_t rsp);

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

void init_sleeping_processes();
void remove_sleeping_process(uint32_t pid);
void sleep(uint64_t sleep_ticks);

#endif
