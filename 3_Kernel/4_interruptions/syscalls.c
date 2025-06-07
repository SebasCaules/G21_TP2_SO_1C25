#include <syscalls.h>
#include <stdint.h>



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

    // Synchronization related syscalls
    (syscall_fn)sys_sem_open,          // 25
    (syscall_fn)sys_sem_close,         // 26
    (syscall_fn)sys_sem_wait,          // 27
    (syscall_fn)sys_sem_post,          // 28

    // IPC related syscalls
    (syscall_fn)sys_create_pipe,       // 29
    (syscall_fn)sys_destroy_pipe,      // 30
    (syscall_fn)sys_read_pipe,         // 31
    (syscall_fn)sys_write_pipe,        // 32
    (syscall_fn)sys_ps,                // 33

    (syscall_fn)sys_total_cpu_ticks,   // 34
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

int64_t sys_sleep(uint64_t milliseconds) {
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
    if (fd < 0 || length <= 0)
		return -1;

	if (fd == STDIN) {
		int fds[2];
		getFds(fds);
		if (fds[0] != STDIN) {
			int64_t read = readPipe(fds[0], (char *)buffer, length);
			return read == 0 ? EOF : read;
		}
		else {
			for (int i = 0; i < length; i++) {
				char c = kb_getchar();
				if (c == 0) { 
					return i;
				}
				buffer[i] = c;
			}
			return length;
		}
	}
	// if the file descriptor is not STDIN, it must be a pipe
	int64_t read = readPipe(fd, (char *)buffer, length);
	return read == 0 ? EOF : read;
}

int64_t sys_write(uint64_t fd, uint16_t * buffer, uint64_t length) {
    if (fd < 1 || length <= 0)
        return -1;

    if (fd < BUILTIN_FDS) {
        int fds[2];
        getFds(fds);
        if (fds[1] > BUILTIN_FDS) {
            return writePipe(fds[1], (const char *)buffer, length);
        }
        return printStrByLength((char *)buffer, fileDescriptorStyle[fd], 0x00000000, length);
    }
    // if the file descriptor is not a builtin FD, it must be a pipe
    return writePipe(fd, (const char *)buffer, length);
}

int64_t sys_clear() {
    clear();
    return 0;
}

int64_t sys_draw_rectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint32_t color) {
    drawRectangle(x, y, width, height, color);
    return 0;
}

int64_t sys_tick() {
    return ticks_elapsed();
}

int64_t sys_reset_cursor() {
    resetCursor();
    return 0;
}

int64_t sys_get_regs(uint64_t * r) {
    return getRegisters(r);
}

int64_t sys_beep(uint64_t freq, uint64_t milliseconds) {
    beep(freq, milliseconds);
    return 0;
}

int64_t sys_draw_pixel(uint64_t x, uint64_t y, uint32_t color) {
    putPixel(color, x, y);
    return 0;
}

// Memory Manager related syscalls

int64_t sys_my_malloc(uint64_t size) {
    return (uint64_t) my_malloc(size);
}

int64_t sys_my_free(uint64_t ptr) {
    my_free((void *) ptr);
    return 0;
}

int64_t sys_mem_dump() {
    return (uint64_t) mem_dump();
}

// Scheduler related syscalls

int64_t sys_new_process(uint64_t main, char** argv, char* name, uint8_t unkillable, int* fileDescriptors) {
    return addProcess((entry_point_t) main, argv, name, unkillable, fileDescriptors);
}

int64_t sys_exit(int64_t retValue) {
    myExit(retValue);
    return 0;
}

int64_t sys_get_pid(void) {
    return getPid();
}

int64_t sys_process_status(void) {
    return (uint64_t) processStatus();
}

int64_t sys_kill_process(uint16_t pid) {
    return killProcess(pid);
}

int64_t sys_set_priority(uint16_t pid, uint8_t priority) {
    return setPriority(pid, priority);
}

int64_t sys_block_process(uint16_t pid) {
    return blockProcess(pid);
}

int64_t sys_unblock_process(uint16_t pid) {
    return unblockProcess(pid);
}

int64_t sys_yield() {
    yield();
    return 0;
}

int64_t sys_waitpid(uint32_t pid) {
    return waitPid(pid);
}

int64_t sys_ps() {
    return (int64_t)processStatus();
}

int64_t sys_total_cpu_ticks() {
    return getTotalCPUTicks();
}

// Synchronization related syscalls

int64_t sys_sem_open(char *name, uint64_t initialValue) {
	return semOpen(name, initialValue);
}

int64_t sys_sem_close(char *name) {
	return semClose(name);
}

int64_t sys_sem_wait(char *name) {
	return semWait(name);
}

int64_t sys_sem_post(char *name) {
	return semPost(name);
}

// IPC related syscalls

int64_t sys_create_pipe(int fds[2]) {
	return createPipe(fds);
}

int64_t sys_destroy_pipe(int writeFd) {
	destroyPipe(writeFd);
	return 1;
}

int64_t sys_read_pipe(int fd, char *buffer, int bytes) {
    return readPipe(fd, buffer, bytes);
}

int64_t sys_write_pipe(int fd, const char *buffer, int bytes) {
    return writePipe(fd, buffer, bytes);
}