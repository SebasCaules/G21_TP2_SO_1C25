#include <stdint.h>
#include <videoDriver.h>
#include <keyboard.h>
#include <syscalls.h>
#include <rtc.h>
#include <time.h>
#include <audioDriver.h>
#include <naiveConsole.h>


#define SYSCALL_COUNT (sizeof(syscall_table) / sizeof(syscall_fn))

extern uint64_t regs[17];

typedef uint64_t (*syscall_fn)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);
typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx, rax;
} Registers;

syscall_fn syscall_table[] = {
    (syscall_fn)sys_sleep,             // 0
    (syscall_fn)sys_time,              // 1
    (syscall_fn)sys_set_font_scale,    // 2
    (syscall_fn)sys_read,              // 3
    (syscall_fn)sys_write,             // 4
    (syscall_fn)sys_clear,             // 5
    (syscall_fn)sys_draw_rectangle,    // 6
    (syscall_fn)sys_tick,              // 7
    (syscall_fn)sys_reset_cursor,      // 8
    (syscall_fn)sys_get_regs,          // 9
    (syscall_fn)sys_beep,              // 10
    (syscall_fn)sys_draw_pixel,        // 11

    // Memory Manager related syscalls
    (syscall_fn)sys_my_malloc,         // 12
    (syscall_fn)sys_my_free,           // 13
    (syscall_fn)sys_mem_dump,          // 14
    
    // Scheduler related syscalls
    (syscall_fn)sys_new_process,       // 15
    (syscall_fn)sys_exit,              // 16
    (syscall_fn)sys_kill_process,      // 17
    (syscall_fn)sys_get_pid,           // 18
    (syscall_fn)sys_process_status,    // 19
    (syscall_fn)sys_set_priority,      // 20
    (syscall_fn)sys_block_process,     // 21
    (syscall_fn)sys_unblock_process,   // 22
    (syscall_fn)sys_yield,             // 23
    (syscall_fn)sys_waitpid,           // 24
};

uint64_t sysCallHandler(Registers * regs) {
    if (regs->rax < SYSCALL_COUNT) {
        return syscall_table[regs->rax](
            (uint64_t)regs->rdi,
            (uint64_t)regs->rsi,
            (uint64_t)regs->rdx,
            (uint64_t)regs->rcx,
            (uint64_t)regs->r8
        );
    }
    return 0;
}

uint64_t sys_sleep(uint64_t milliseconds) {
    unsigned long long initial_time = ms_elapsed();
    unsigned long long currentTime = initial_time;
    _sti();
    while ((currentTime - initial_time) <= milliseconds) {
        currentTime = ms_elapsed();
    }
    _cli();
    return 1;
}

int64_t sys_time(time_struct* time) {
    time->seconds = getRTCSeconds();
    time->minutes = getRTCMinutes();
    time->hour = getRTCHours();
    time->day = getRTCDayOfMonth();
    time->month = getRTCMonth();
    time->year = getRTCYear();
    return 0;
}

int64_t sys_set_font_scale(uint64_t scale) {
    return setFontScale(scale);
}

int64_t sys_read(uint64_t fd, uint16_t * buffer, uint64_t length) {
    unsigned char character;
    uint64_t i = 0;
    while (i < length && (character = bufferNext()) != 0) {
        if (character == '\r' || character == '\n') {
            buffer[i++] = '\n';  // Store newline in buffer
            break;               // Exit on newline
        }
        buffer[i++] = character;
    }
    return i;
}

int64_t sys_write(uint64_t fd, uint16_t * buffer, uint64_t length) {
    uint32_t fileDescriptorStyle[] = {0, 0x00FFFFFF, 0x00FF0000, 0x0000FF00};
    return printStrByLength((char *)buffer, fileDescriptorStyle[fd], 0x00000000, length);
    // return ncprintStrByLength((char *)buffer, fileDescriptorStyle[fd], 0x00000000, length);

}

int64_t sys_clear() {
    clear();
    return 0;
}

int64_t sys_draw_rectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint32_t color) {
    drawRectangle(x, y, width, height, color);
    return 0;
}

uint64_t sys_tick() {
    return ticks_elapsed();
}

uint64_t sys_reset_cursor() {
    resetCursor();
    return 0;
}

uint64_t sys_get_regs(uint64_t * r) {
    return getRegisters(r);
}

uint64_t sys_beep(uint64_t freq, uint64_t milliseconds) {
    beep(freq, milliseconds);
    return 0;
}

uint64_t sys_draw_pixel(uint64_t x, uint64_t y, uint32_t color) {
    putPixel(color, x, y);
    return 0;
}

// Memory Manager related syscalls

void *sys_my_malloc(uint32_t size) {
    return my_malloc(size);
}

uint16_t sys_my_free(void *ptr) {
    my_free(ptr);
    return 0;
}

mem_info_t *sys_mem_dump() {
    return mem_dump();
}

// Scheduler related syscalls

int64_t sys_new_process(entry_point_t main, char** argv, char* name, uint8_t unkillable, int* fileDescriptors) {
    return addProcess(main, argv, name, unkillable, fileDescriptors);
}

int64_t sys_exit(int64_t retValue) {
    myExit(retValue);
    return 0;
}

uint16_t sys_get_pid(void) {
    return getPid();
}

process_info_t* sys_process_status(void) {
    return processStatus();
}

int32_t sys_kill_process(uint16_t pid) {
    return killProcess(pid);
}

int sys_set_priority(uint16_t pid, uint8_t priority) {
    return setPriority(pid, priority);
}

int sys_block_process(uint16_t pid) {
    return blockProcess(pid);
}

int sys_unblock_process(uint16_t pid) {
    return unblockProcess(pid);
}

uint16_t sys_yield(void) {
    yield();
    return 0;
}

int64_t sys_waitpid(uint32_t pid) {
    return waitPid(pid);
}