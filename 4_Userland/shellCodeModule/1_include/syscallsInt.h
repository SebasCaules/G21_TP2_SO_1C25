#ifndef TPE_ARQUI_SYSCALLS_H
#define TPE_ARQUI_SYSCALLS_H

#include <stdint.h>

typedef struct time {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minutes;
    uint8_t seconds;

} time_struct;

/**
 * @brief Retrieves the current system time.
 * @param time A pointer to a time_struct to store the current time.
 * @return 0 on success, or -1 on failure.
 */
int64_t sys_time(time_struct* time);

/**
 * @brief Sets the font scale for the display.
 * @param scale The desired font scale.
 * @return 0 on success, or -1 on failure.
 */
int64_t sys_set_font_scale(uint64_t scale);

/**
 * @brief Reads data from a file descriptor into a buffer.
 * @param fd The file descriptor to read from.
 * @param buffer The buffer to store the read data.
 * @param amount The number of bytes to read.
 * @return The number of bytes read, or -1 on error.
 */
int64_t sys_read(uint64_t fd, uint16_t * buffer, uint64_t amount);

/**
 * @brief Writes data from a buffer to a file descriptor.
 * @param fd The file descriptor to write to.
 * @param buffer The buffer containing data to write.
 * @param amount The number of bytes to write.
 * @return The number of bytes written, or -1 on error.
 */
int64_t sys_write(uint64_t fd, uint16_t * buffer, uint64_t amount);

/**
 * @brief Clears the display screen.
 * @return 0 on success, or -1 on failure.
 */
int64_t sys_clear();

/**
 * @brief Draws a rectangle on the screen at the specified position and size.
 * @param x The x-coordinate of the rectangle's starting point.
 * @param y The y-coordinate of the rectangle's starting point.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle in hexadecimal format.
 * @return 0 on success, or -1 on failure.
 */
int64_t sys_draw_rectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint32_t color);

/**
 * @brief Retrieves the system tick count.
 * @return The number of ticks since the system started.
 */
uint64_t sys_tick();

/**
 * @brief Suspends execution for a specified number of milliseconds.
 * @param milliseconds The number of milliseconds to sleep.
 * @return 0 on success, or -1 on failure.
 */
uint64_t sys_sleep(uint64_t milliseconds);

/**
 * @brief Resets the cursor to the initial screen position.
 * @return 0 on success, or -1 on failure.
 */
uint64_t sys_reset_cursor();

/**
 * @brief Retrieves the values of the system's general-purpose registers.
 * @param r A pointer to an array to store register values.
 * @return 0 on success, or -1 on failure.
 */
uint64_t sys_get_regs(uint64_t * r);

/**
 * @brief Plays a sound using the system speaker at a given frequency and duration.
 * @param freq The frequency of the sound in Hertz.
 * @param milliseconds The duration of the sound in milliseconds.
 * @return 0 on success, or -1 on failure.
 */
uint64_t sys_beep(uint64_t freq, uint64_t milliseconds);

/**
 * @brief Draws a pixel on the screen at a specified (x, y) position with a given color.
 * @param x The x-coordinate of the pixel position.
 * @param y The y-coordinate of the pixel position.
 * @param color The color of the pixel in hexadecimal format (e.g., 0xRRGGBB).
 * @return 0 on success, or -1 on failure.
 */
uint64_t sys_draw_pixel(uint64_t x, uint64_t y, uint32_t color);

#endif
