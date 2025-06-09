#include <lib.h>
#include <memoryManager.h>
#include <process.h>
#include <syscalls.h>
#include <stddef.h>
#include <syscalls.h>

#define STACK_SIZE 0x1000 // 4 KiB


extern void *setup_stack_frame(void (*wrapper)(entry_point_t, char **),
									 entry_point_t main, void *stackEnd,
									 void *argv);

/*
; En assembler (_setup_stack_frame)
; Crea algo como:
----------------------
| Ret addr (wrapper) | <- nuevo RSP
| argv               |
| entry_point        |
| ...                |
*/

static char **duplicateArgv(char **argv);
static int argcFromArgv(char **argv);
static void freeArgv(char **argv);
void processCaller(entry_point_t main, char **args);

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
) {
    process_t *p = my_malloc(sizeof(process_t));
    if (p == NULL)
        return NULL;
    
    p->pid = pid;
    p->parent_pid = parent_pid;
    p->waiting_for_pid = 0; // No waiting for any process initially
    p->priority = priority;
    p->remaining_quantum = priority; // Quantum is initially set to priority
    p->unkillable = unkillable;
    p->status = READY;
    p->fd_out = fd_out;
    p->fd_in = fd_in;
    p->cpuTicks = 0;

    p->stack_base = my_malloc(STACK_SIZE);
    if (p->stack_base == NULL) {
        my_free(p);
        return NULL;
    }
    p->stack_pointer = p->stack_base + STACK_SIZE; // Stack grows downward

    // Allocate memory for the arguments
    p->argv = duplicateArgv(argv);
    if (p->argv == NULL) {
        my_free(p->stack_base);
        my_free(p);
        return NULL;
    }

    strncopy(p->name, name, MAX_NAME_LENGTH - 1);
    p->name[MAX_NAME_LENGTH - 1] = 0;
    // Setup the initial stack frame
	p->stack_pointer = setup_stack_frame(&processCaller, entry_point, p->stack_pointer, (void *) p->argv);
    return p;
}

/**
 * Libera recursos del proceso (no el stack si lo maneja el MM).
 */
void destroyProcess(process_t *p) {
    if (p == NULL)
        return;
    
    freeArgv(p->argv);
    
    my_free(p->stack_base);
    
    my_free(p);
}

/**
 * Duplicar los argumentos para el proceso.
 */
static char **duplicateArgv(char **argv) {
    if (argv == NULL || argv[0] == NULL) {
        char **new_argv = my_malloc(sizeof(char *));
        if (new_argv == NULL)
            return NULL;
        new_argv[0] = NULL;
        return new_argv;
    }

    int argc = argcFromArgv(argv);

    char **new_argv = my_malloc((argc + 1) * sizeof(char *));
    if (new_argv == NULL)
        return NULL;

    for (int i = 0; i < argc; i++) {
        int len = strlength(argv[i]) + 1;
        new_argv[i] = my_malloc(len);
        if (new_argv[i] == NULL)
            return NULL;
        memcpy(new_argv[i], argv[i], len);
    }

    new_argv[argc] = NULL;
    return new_argv;
}

/**
 * Contar el número de argumentos en el array.
 */
static int argcFromArgv(char **argv) {
    int count = 0;
    while (argv != NULL && argv[count] != NULL)
        count++;
    return count;
}

/**
 * Liberar la memoria de los argumentos.
 */
static void freeArgv(char **argv) {
    if (argv == NULL)
        return;
    
    for (int i = 0; argv[i] != NULL; i++) {
        my_free(argv[i]);
    }
    
    my_free(argv);
}

/**
 * Llama a la función principal del proceso y luego termina el proceso.
 */
void processCaller(entry_point_t main, char **args) {
    int argc = argcFromArgv(args);
    int64_t retValue = main(argc, args);
    myExit(retValue); // Terminate the process with the return value
}
