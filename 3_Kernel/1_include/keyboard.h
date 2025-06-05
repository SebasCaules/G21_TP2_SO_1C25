#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <stdint.h>

extern int getPressedKey();

/**
 * @brief Retrieves the currently pressed key.
 * @return The character representation of the pressed key.
 */
void pressedKey();

/**
 * @brief Gets the next character from the input buffer.
 * @return The next character in the buffer.
 */
unsigned char bufferNext();

/**
 * @brief Retrieves the current values of the CPU registers and stores them in the provided array.
 * @param r Pointer to an array where the register values will be stored.
 * @return 1 or 0
 */
uint64_t getRegisters(uint64_t * r);

char kb_getchar();

void release_stdin();

#endif