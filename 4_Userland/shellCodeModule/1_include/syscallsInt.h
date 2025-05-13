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
 * @brief Causes the program to sleep for a specified number of milliseconds.
 * @param milliseconds The duration to sleep, in milliseconds.
 * @return Returns 0 on success, or an error code on failure.
 */
int64_t sys_sleep(uint64_t milliseconds);

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
int64_t sys_tick();

/**
 * @brief Resets the cursor to its initial position.
 * @return Returns 0 on success, or an error code on failure.
 */
int64_t sys_reset_cursor();

/**
 * @brief Retrieves the current state of the CPU registers.
 * @param r Pointer to a buffer where the register values will be stored.
 * @return Returns 0 on success, or an error code on failure.
 */
int64_t sys_get_regs(uint64_t * r);

/**
 * @brief Produces a beep sound at a specified frequency for a specified duration.
 * @param freq The frequency of the beep in Hertz.
 * @param milliseconds The duration of the beep in milliseconds.
 * @return Returns 0 on success, or an error code on failure.
 */
int64_t sys_beep(uint64_t freq, uint64_t milliseconds);

/**
 * @brief Draws a pixel on the screen at a specified (x, y) position with a given color.
 * @param x The x-coordinate of the pixel position.
 * @param y The y-coordinate of the pixel position.
 * @param color The color of the pixel in hexadecimal format (e.g., 0xRRGGBB).
 * @return 0
 */
int64_t sys_draw_pixel(uint64_t x, uint64_t y, uint32_t color);

// Memory Manager related syscalls
int64_t sys_my_malloc(uint64_t size);

int64_t sys_my_free(uint64_t ptr);

int64_t sys_mem_dump();

/**
 * @brief Creates and schedules a new process in the system.
 * @param main Entry point function of the process.
 * @param argv Arguments passed to the process.
 * @param name Name of the process.
 * @param unkillable If set to 1, the process cannot be killed.
 * @param fileDescriptors Initial file descriptors for the process.
 * @return The PID of the new process on success, or -1 on failure.
 */
int64_t sys_new_process(uint64_t main, char** argv, char* name, uint8_t unkillable, int* fileDescriptors);

/**
 * @brief Retrieves the current state of the CPU registers and stores it in a snapshot.
 * @param snapshot Pointer to a Snapshot structure where the register values will be stored.
 */

int64_t sys_exit(int64_t retValue);

/**
 * @brief Gets the PID of the currently executing process.
 * @return The PID of the current process.
 */
int64_t sys_get_pid(void);

/**
 * @brief Returns an array of information about all current processes.
 * @return A pointer to an array of process_info_t structures.
 */
int64_t sys_process_status(void);

/**
 * @brief Terminates the process with the given PID.
 * @param pid The PID of the process to terminate.
 * @return 0 on success, or -1 if the PID is invalid.
 */
int64_t sys_kill_process(uint16_t pid);

/**
 * @brief Sets the priority of a process.
 * @param pid The PID of the process.
 * @param newPriority The new priority value.
 * @return 0 on success, or -1 if the PID is invalid.
 */
int64_t sys_set_priority(uint16_t pid, uint8_t newPriority);

/**
 * @brief Blocks a process from being scheduled.
 * @param pid The PID of the process to block.
 * @return 0 on success, or -1 if the PID is invalid.
 */
int64_t sys_block_process(uint16_t pid);

/**
 * @brief Unblocks a previously blocked process.
 * @param pid The PID of the process to unblock.
 * @return 0 on success, or -1 if the PID is invalid.
 */
int64_t sys_unblock_process(uint16_t pid);

/**
 * @brief Terminates the currently executing process.
 * @return 0 on success
 */
int64_t sys_yield(void);

/**
 * @brief Waits for a process with a specific PID to terminate.
 * @param pid The PID of the process to wait for.
 * @return The return value of the terminated process, or -1 on error.
 */
int64_t sys_waitpid(uint32_t pid);


#endif
