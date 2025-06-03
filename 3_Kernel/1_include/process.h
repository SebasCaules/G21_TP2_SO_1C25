#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define MAX_NAME_LENGTH 64

typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
} process_status_t;

typedef int (*entry_point_t)(int argc, char **argv);

typedef struct process {
    uint16_t pid;
    int16_t parent_pid;

    process_status_t status;
    uint8_t priority;
    uint8_t remaining_quantum; // necesario para Round Robin con prioridad
    uint8_t unkillable;

    void *stack_base;     // Base de la pila (al final del stack)
    void *stack_pointer;  // Stack pointer actual (para context switch)

    entry_point_t entry_point;
    char **argv;
    int argc;

    char name[MAX_NAME_LENGTH];

    int32_t return_value;
    int16_t waiting_for_pid; // -1 si no espera

    // opcionales: stdin/stdout, foreground, etc.
    uint16_t fd_in;
    uint16_t fd_out;

    uint8_t waiting_for_stdin;
} process_t;

/**
 * Crea una nueva estructura de proceso.
 * No realiza scheduling ni asignación de PID.
 */
process_t *createProcess(
    uint16_t pid,
    int16_t parent_pid,
    uint16_t waiting_for_pid,
    entry_point_t entry_point,
    char **argv,
    const char *name,
    uint8_t priority,
    uint8_t unkillable,
    uint16_t fd_in,
    uint16_t fd_out
);

/**
 * Libera recursos del proceso (no el stack si lo maneja el MM).
 */
void destroyProcess(process_t *p);

#endif // PROCESS_H
