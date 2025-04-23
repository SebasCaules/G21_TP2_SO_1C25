#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

typedef enum {
    STDIN = 0,
    STDOUT,
    STDERR,
    STDMARK,
} fileDesc;

typedef struct time {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minutes;
    uint8_t seconds;
} time_struct;

typedef struct {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rip;
} Snapshot;

/**
 * @brief Causes the program to sleep for a specified number of milliseconds.
 * @param milliseconds The duration to sleep, in milliseconds.
 * @return Returns 0 on success, or an error code on failure.
 */
uint64_t sys_sleep(uint64_t milliseconds);

/**
 * @brief Retrieves the current time and stores it in the provided time structure.
 * @param time Pointer to a time_struct where the current time will be stored.
 * @return Returns 0 on success, or an error code on failure.
 */
int64_t sys_time(time_struct* time);

/**
 * @brief Reads data from a specified file descriptor into a buffer.
 * @param fd The file descriptor to read from.
 * @param buffer Pointer to the buffer where the read data will be stored.
 * @param length The number of bytes to read.
 * @return Returns the number of bytes read, or an error code on failure.
 */
int64_t sys_read(uint64_t fd, uint16_t * buffer, uint64_t length);

/**
 * @brief Writes data to a specified file descriptor from a buffer.
 * @param fd The file descriptor to write to.
 * @param buffer Pointer to the buffer containing the data to write.
 * @param length The number of bytes to write.
 * @return Returns the number of bytes written, or an error code on failure.
 */
int64_t sys_write(uint64_t fd, uint16_t * buffer, uint64_t length);

/**
 * @brief Clears the terminal or display screen.
 * @return Returns 0 on success, or an error code on failure.
 */
int64_t sys_clear();

/**
 * @brief Sets the font scale for text rendering.
 * @param scale The desired font scale.
 * @return Returns 0 on success, or an error code on failure.
 */
int64_t sys_set_font_scale(uint64_t scale);

/**
 * @brief Draws a rectangle on the display at the specified position and size.
 * @param x The x-coordinate of the rectangle's top-left corner.
 * @param y The y-coordinate of the rectangle's top-left corner.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle.
 * @return Returns 0 on success, or an error code on failure.
 */
int64_t sys_draw_rectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint32_t color);

/**
 * @brief Returns the current system tick count.
 * @return The current tick count.
 */
uint64_t sys_tick();

/**
 * @brief Resets the cursor to its initial position.
 * @return Returns 0 on success, or an error code on failure.
 */
uint64_t sys_reset_cursor();

/**
 * @brief Retrieves the current state of the CPU registers.
 * @param r Pointer to a buffer where the register values will be stored.
 * @return Returns 0 on success, or an error code on failure.
 */
uint64_t sys_get_regs(uint64_t * r);

/**
 * @brief Produces a beep sound at a specified frequency for a specified duration.
 * @param freq The frequency of the beep in Hertz.
 * @param milliseconds The duration of the beep in milliseconds.
 * @return Returns 0 on success, or an error code on failure.
 */
uint64_t sys_beep(uint64_t freq, uint64_t milliseconds);

/**
 * @brief Draws a pixel on the screen at a specified (x, y) position with a given color.
 * @param x The x-coordinate of the pixel position.
 * @param y The y-coordinate of the pixel position.
 * @param color The color of the pixel in hexadecimal format (e.g., 0xRRGGBB).
 * @return 0
 */
uint64_t sys_draw_pixel(uint64_t x, uint64_t y, uint32_t color);

#endif // !SYSCALLS_H

