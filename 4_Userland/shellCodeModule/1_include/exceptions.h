#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

/**
 * @brief Triggers a divide-by-zero exception.
 *        Typically used for testing exception handling mechanisms.
 */
void divZero();

/**
 * @brief Triggers an invalid opcode exception.
 *        Used to simulate an illegal operation for exception testing.
 */
void opCode();

#endif // EXCEPTIONS_H