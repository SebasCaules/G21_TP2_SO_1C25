#ifndef PROCESS_INC_H
#define PROCESS_INC_H

#include <stdint.h>
#include <stdbool.h>


#define MAX_NAME_LENGTH 64
#define MAX_PROCESSES 64

#define NO_PID -1
#define INIT_PID 0
#define SHELL_PID 1

#define MAX_PRIORITY 4
#define MIN_PRIORITY 1

typedef enum {
    READY,
    BLOCKED,
    RUNNING,
    TERMINATED
} process_status_t;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int16_t pid;
    int16_t ppid;
    uint8_t priority;
    void *stackBase;
    void *stackPointer;
    bool foreground;
    process_status_t status;
    uint64_t cpuTicks; // para calcular el %CPU (eventualmente)
} process_info_t;

#endif // PROCESS_INC_H
