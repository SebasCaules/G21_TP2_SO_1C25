#ifndef TPE_ARQUI_AUDIODRIVER_H
#define TPE_ARQUI_AUDIODRIVER_H

#include <stdint.h>
#include <syscalls.h>
#include <keyboard.h>

/**
 * @brief Plays a sound at a specified frequency for a given duration.
 * @param freq Frequency of the sound in Hertz.
 * @param milliseconds Duration of the sound in milliseconds.
 */
void beep(uint64_t freq, uint64_t milliseconds);

#endif //TPE_ARQUI_AUDIODRIVER_H