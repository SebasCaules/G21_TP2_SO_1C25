#include <scheduler.h>
#include <memoryMap.h>
#include <process.h>
#include <stddef.h>
#include <time.h>
#include "syscalls.h"
#include "keyboard.h"

typedef struct scheduler_t {
    process_t* processes[MAX_PROCESSES + 1];
    uint16_t current;
    uint8_t size;
} scheduler_t;

extern void _hlt();
extern void timer_tick();

static schedulerADT scheduler = NULL;
static uint8_t force_reschedule = 0;

static int initMain(int argc, char **argv);
static process_t* getNextProcess(void);
static void removeProcess(uint16_t pid);
static void adoptChildren(int16_t pid);

schedulerADT initScheduler(void) {
    if (scheduler != NULL) {
        return scheduler;
    }
    scheduler = (schedulerADT) SCHEDULER_ADDRESS;

    for (int i = 0; i < MAX_PROCESSES; i++) {
        scheduler->processes[i] = NULL;
    }

    scheduler->current = NO_PID;
    scheduler->size = 0;

    char *argv[] = { NULL };
    int fds[2] = { STDIN, STDOUT };
    addProcess((entry_point_t) initMain, argv, "init", 1, fds);
    // sys_write(STDOUT, (uint16_t *)"Scheduler initialized.\n", 24);
    return scheduler;
}

schedulerADT getScheduler(void) {
    return scheduler;
}

void* schedule(void* prevRSP) {
    if (scheduler == NULL || scheduler->size == 0) {
        return prevRSP;
    }

    if (scheduler->current != NO_PID) {
		process_t *currentProcess = scheduler->processes[scheduler->current];
		currentProcess->stack_pointer = prevRSP;
		if (currentProcess->status == RUNNING) {
			currentProcess->status = READY;
		}
	}

    process_t *next = getNextProcess();
    if (next == NULL) {
        return prevRSP;
    }
    scheduler->current = next->pid;
    uint64_t nextRSP = (uint64_t) next->stack_pointer;
    next->status = RUNNING;

    return (void *) nextRSP;

}

int64_t addProcess(entry_point_t main, char** argv, char* name, uint8_t unkillable, int* fileDescriptors) {
    if (scheduler == NULL || scheduler->size >= MAX_PROCESSES || fileDescriptors == NULL) {
        return -1;
    }


    uint16_t pid = NO_PID;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (scheduler->processes[i] == NULL) {
            pid = i;
            break;
        }
    }
    if (pid == NO_PID) {
        return -1;
    }
    
    uint16_t parentPid = (scheduler->current != NO_PID) ? scheduler->current : NO_PID;
    
    process_t *newProcess = createProcess(
        pid,
        parentPid,
        NO_PID,
        main,
        argv,
        name,
        MIN_PRIORITY,
        unkillable,
        fileDescriptors[STDIN],
        fileDescriptors[STDOUT]
    );

    if (newProcess == NULL) {
        return NO_PID;
    }
    newProcess->waiting_for_stdin = 0;

    scheduler->processes[pid] = newProcess;
    scheduler->size++;

    return pid;

}


void destroyScheduler(void) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (scheduler->processes[i] != NULL) {
            destroyProcess(scheduler->processes[i]);
        }
    }
    scheduler = NULL;
}


/*
Qué debería hacer:
* Marcar el proceso actual como READY.
* Forzar una llamada a schedule() en la próxima interrupción (o manualmente si se quiere inmediato).
* Puede ser tan simple como llamar a schedule(current_rsp); y hacer un return de ahí si el sistema lo permite.
*/
void yield(void) {
    force_reschedule = 1;
    timer_tick();
}


uint16_t getPid(void) {
    return scheduler->current;
}

/*
Qué debería hacer:
* Cambiar el estado del proceso a TERMINATED.
* Llamar a adoptChildren(pid).
* Si otro proceso espera su finalización, desbloquearlo.
* Si es el proceso actual, forzar un cambio de contexto.
*/
int32_t killProcess(uint16_t pid) {
    if (scheduler == NULL || pid >= MAX_PROCESSES || scheduler->processes[pid] == NULL || scheduler->processes[pid]->unkillable) {
        return -1;
    }
    
    adoptChildren(pid);
    process_t *process = scheduler->processes[pid];
    process_t *parent = scheduler->processes[process->parent_pid];
    if (parent != NULL && parent->status == BLOCKED && parent->waiting_for_pid == process->pid) {
        unblockProcess(process->parent_pid);
    }
    uint8_t contextSwitch = scheduler->processes[pid]->status == RUNNING;
    remove_sleeping_process(pid);
    remove_process_from_all_semaphore_queues(pid);
    if (process->fd_out != STDOUT) {
		send_pipe_eof(process->fd_out);
	}
    if (process->waiting_for_stdin) {
		release_stdin();
	}
    removeProcess(pid);

    if (contextSwitch) {
        yield();
    }
    return 0;
}


int32_t killCurrentProcess(void) {
    return killProcess(scheduler->current);
}


int blockProcess(uint16_t pid) {
    return sleepBlock(pid, 0);
}


int unblockProcess(uint16_t pid) {
    if (scheduler == NULL || pid >= MAX_PROCESSES || scheduler->processes[pid] == NULL || scheduler->processes[pid]->status != BLOCKED) {
        return -1;
    }

    remove_sleeping_process(pid);
    scheduler->processes[pid]->status = READY;

    return 0;
}


int setPriority(uint16_t pid, uint8_t newPriority) {
    if (scheduler == NULL || pid >= MAX_PROCESSES || 
        scheduler->processes[pid] == NULL || 
        newPriority < MIN_PRIORITY || newPriority > MAX_PRIORITY) {
        return -1;
    }

    scheduler->processes[pid]->priority = newPriority;
    scheduler->processes[pid]->remaining_quantum = newPriority;
    return 0;
}
// static process_info_t psInfo[32 + 1];

process_info_t* processStatus(void) {
    static process_info_t processInfo[MAX_PROCESSES];
    int index = 0;

    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_t* proc = scheduler->processes[i];
        if (proc != NULL) {
            process_info_t* info = &processInfo[index++];
            strncopy(info->name, proc->name, MAX_NAME_LENGTH);
            info->pid = proc->pid;
            info->ppid = proc->parent_pid;
            info->priority = proc->priority;
            info->stackBase = proc->stack_base;
            info->stackPointer = proc->stack_pointer;
            info->foreground = (proc->fd_in == STDIN && proc->fd_out == STDOUT);
            info->status = proc->status;
            info->cpuTicks = 0;  // para trackear el uso de CPU (eventualmente)
        }
    }

    process_info_t empty;
    empty.pid = -1;
    processInfo[index] = empty;

    return processInfo;
}


void myExit(int64_t retValue) {
    if (scheduler == NULL) {
        return;
    }
    
    process_t *currentProcess = scheduler->processes[scheduler->current];

    if (currentProcess->fd_out != STDOUT) {
		send_pipe_eof(currentProcess->fd_out);
	}
    
    currentProcess->status = TERMINATED;
    currentProcess->return_value = retValue;
    remove_sleeping_process(currentProcess->pid);

    process_t *parent = scheduler->processes[currentProcess->parent_pid];
    if (parent != NULL && parent->status == BLOCKED &&
        parent->waiting_for_pid == currentProcess->pid) {
        unblockProcess(parent->pid);
    }

    // removeProcess(currentProcess->pid); // Eliminar el proceso correctamente
    yield();
}


int64_t waitPid(uint32_t pid) {
    if (scheduler == NULL || pid >= MAX_PROCESSES || 
        scheduler->processes[pid] == NULL || 
        scheduler->processes[pid]->parent_pid != scheduler->current) {
        return -1;
    }

    if (scheduler->processes[pid]->status != TERMINATED) {
        scheduler->processes[scheduler->current]->waiting_for_pid = pid;
        blockProcess(scheduler->current);
    }

    scheduler->processes[scheduler->current]->waiting_for_pid = NO_PID;
    int64_t retValue = scheduler->processes[pid]->return_value;
    removeProcess(pid);

    return retValue;
}


void getFds(int* fds) {
    if (scheduler == NULL || scheduler->current == NO_PID) {
        return;
    }
    process_t *currentProcess = scheduler->processes[scheduler->current];
    fds[STDIN] = currentProcess->fd_in;
    fds[STDOUT] = currentProcess->fd_out;
}


int sleepBlock(uint16_t pid, uint8_t sleep) {
    if(pid == 0 || scheduler == NULL || pid >= MAX_PROCESSES || scheduler->processes[pid] == NULL || scheduler->processes[pid]->status == TERMINATED) {
        return -1;
    }

    if (sleep == 0) {
        remove_sleeping_process(pid);
    }

    uint8_t contextSwitch = pid == scheduler->current;
	scheduler->processes[pid]->status = BLOCKED;

	if (contextSwitch) {
		yield();
	}
	return 0;
}


void killForegroundProcess(void) {
    if (scheduler == NULL) {
        return;
    }
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (scheduler->processes[i] != NULL && scheduler->processes[SHELL_PID]->waiting_for_pid == i) {
            killProcess(i);
            return;
        }
    }
}


void updateStdinWait(uint8_t value) {
    if (scheduler == NULL || scheduler->current == NO_PID) {
        return;
    }
    scheduler->processes[scheduler->current]->waiting_for_stdin = value;
}


static int initMain(int argc, char **argv) {
    char **shellArgs = { NULL };
    int fds[2] = { STDIN, STDOUT };

    int shellPid = addProcess((entry_point_t) SHELL_ADDRESS, shellArgs, "shell", 1, fds);
    setPriority(shellPid, MAX_PRIORITY);
    // sys_write(STDOUT, (uint16_t *)"shell process started.\n", 22);
    
    while (1) {
		for (int i = 0; i < MAX_PROCESSES; i++) {
			if (scheduler->processes[i] != NULL) {
				if (scheduler->processes[i]->status == TERMINATED &&
					scheduler->processes[i]->parent_pid == 0) {
					removeProcess(i);
				}
			}
		}
		_hlt();
	}
    return 0;
}

static process_t* getNextProcess(void) {
    if (scheduler == NULL || scheduler->size == 0) {
        return NULL;
    }

    // Ver si el proceso actual puede seguir ejecutándose
    process_t *currentProcess = scheduler->processes[scheduler->current];
    if (currentProcess != NULL &&
		(currentProcess->status == READY || currentProcess->status == RUNNING) &&
		currentProcess->remaining_quantum > 0 &&
        !force_reschedule) {
		currentProcess->remaining_quantum--;
		return currentProcess;
	}

    // Buscar el próximo proceso listo (Round Robin)
    uint16_t start = (scheduler->current == NO_PID) ? 0 : (scheduler->current + 1) % MAX_PROCESSES;
    uint16_t current = start;

    do {
        process_t *candidate = scheduler->processes[current];
        if (candidate != NULL && candidate->status == READY) {
            scheduler->current = current;
            candidate->remaining_quantum = candidate->priority;
            force_reschedule = 0;
            return candidate;
        }
        current = (current + 1) % MAX_PROCESSES;
    } while (current != start);
    force_reschedule = 0;
    return NULL;
}

static void adoptChildren(int16_t pid) {
	for (int i = 0; i < MAX_PROCESSES; i++) {
		if (scheduler->processes[i] != NULL &&
			scheduler->processes[i]->parent_pid == pid) {
			scheduler->processes[i]->parent_pid = 0;
		}
	}
}

static void removeProcess(uint16_t pid) {
    if (scheduler->processes[pid] == NULL) {
        return; // Proceso ya eliminado
    }
    destroyProcess(scheduler->processes[pid]);
    scheduler->processes[pid] = NULL;
    scheduler->size--;
}