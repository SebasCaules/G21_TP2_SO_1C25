#include <stdint.h>
#include <strLib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "syscallsInt.h"
#include "colors.h"
#include "defs.h"
#include "processInc.h"
#include "memoryManagerInc.h"


#ifndef TPE_ARQUI_STDLIB_H
#define TPE_ARQUI_STDLIB_H
#define VRGCLI


typedef enum {
	INT_TYPE = 0,
    HEX_TYPE,
    BIN_TYPE,
    OCT_TYPE,
	FLOAT_TYPE,
	DOUBLE_TYPE,
	CHAR_TYPE,
	STR_TYPE,
} Types;


/**
 * @brief Prints formatted output to the specified file descriptor.
 * @param fd The file descriptor to print to.
 * @param fmt The format string.
 * @param ... Additional arguments for format specifiers.
 * @return The number of characters printed.
 */
int fdprintf(uint64_t fd, const char* fmt, ...);

/**
 * @brief Prints formatted output to the standard output.
 * @param str The format string.
 * @param ... Additional arguments for format specifiers.
 * @return The number of characters printed.
 */
int	printf(const char * str, ...);


/**
 * @brief Reads a single character from standard input.
 * @return The character read.
 */
int getchar();


/**
 * @brief Reads a single character input.
 * @param c Pointer to the character to store the input.
 * @return 1 if a character was read, 0 otherwise.
 */
int readInput(char * c);

/**
 * @brief Writes a single character to the standard output.
 * @param c The character to write.
 * @return The character written.
 */
int putchar(char c);

/**
 * @brief Reads formatted input from standard input.
 * @param format The format string.
 * @param ... Additional arguments for storing the input values.
 * @return The number of items successfully read.
 */
int scanf(const char *format, ...);

/**
 * @brief Outputs a string without a newline.
 * @param str The string to output.
 * @return The number of characters printed.
 */
int putsNoNewLine(const char *str);

/**
 * @brief Outputs a string followed by a newline.
 * @param str The string to output.
 * @return The number of characters printed.
 */
int puts(const char *str);

/**
 * @brief Reads a line of input into a buffer.
 * @param buffer The buffer to store the input string.
 * @param n The maximum number of characters to read.
 * @return The number of characters read.
 */
int gets(char *buffer, int n);

/**
 * @brief Sets the font scale for the display.
 * @param scale The font scale to apply.
 * @return 0 on success, non-zero on error.
 */
int setFontScale(int scale);

/**
 * @brief Clears the current display or view.
 */
void clearView();

/**
 * @brief Draws a filled rectangle on the display.
 * @param x The x-coordinate of the top-left corner.
 * @param y The y-coordinate of the top-left corner.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle.
 */
void drawRectangle(int x, int y, int width, int height, int color);

/**
 * @brief Pauses the program for a specified duration.
 * @param milliseconds The time to sleep in milliseconds.
 */
void sleep(int milliseconds);

/**
 * @brief Resets the cursor to the starting position.
 */
void resetCursor();


/**
 * @brief Plays a beep sound at a specified frequency and duration.
 * @param freq The frequency of the beep in Hz.
 * @param milliseconds The duration of the beep in milliseconds.
 */
void beep(int freq, int milliseconds);

/**
 * @brief Converts a number from one base to another.
 * @param initBase The initial base of the number.
 * @param finalBase The desired base of the number.
 * @param num The number to be converted, represented as a string.
 */
char* convert(int argc, char *argv[]);

// Cambiar por strToInt
int64_t satoi (char * str, int64_t * flag);

void * my_malloc(uint64_t size);

void my_free(void *ptr);

int mem(int argc, char *argv[]);

int64_t newProcess(EntryPoint main, char** argv, char* name, uint8_t unkillable, int* fileDescriptors);

int64_t exit(int64_t retValue);

int64_t getPid(void);

int64_t processStatus(void);

int64_t kill(uint64_t pid);

int64_t setPriority(uint16_t pid, uint8_t newPriority);

int64_t blockProcess(uint16_t pid);

int64_t unblockProcess(uint16_t pid);

int64_t yield(void);

int64_t waitPid(uint32_t pid);

int ps(int argc, char *argv[]);

int64_t nice(uint64_t pid, uint64_t newPriority);

int64_t semOpen(char *name, int initialValue);

int64_t semClose(char *name);

int64_t semWait(char *name);

int64_t semPost(char *name);

int64_t createPipe(uint64_t fds[2]);

int64_t destroyPipe(uint64_t writeFd);

int64_t readPipe(uint64_t fd, char *buffer, int bytes);

int64_t writePipe(uint64_t fd, const char *buffer, int bytes);


#endif //TPE_ARQUI_STDLIB_H
