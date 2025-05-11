#include <scheduler.h>
#include <memoryMap.h>
#include <process.h>
#include <stddef.h>

#define NO_PID -1
#define STDOUT 1
#define STDIN 0
#define INIT_PID 0

typedef struct scheduler_t {
    process_t* processes[MAX_PROCESSES];
    uint16_t current;
    uint8_t size;
} scheduler_t;

extern void _hlt();

static schedulerADT scheduler = NULL;

static int initMain(int argc, char **argv);
static process_t* getNextProcess(void);
static void removeProcess(uint16_t pid);

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

    return scheduler;

}


schedulerADT getScheduler(void) {
    return scheduler;
}


void* schedule(void* prevRSP) {
    if (scheduler == NULL || scheduler->size == 0) {
        return prevRSP;
    }

    if (scheduler->current != NO_PID && scheduler->processes[scheduler->current] != NULL) {
        process_t* currentProcess = scheduler->processes[scheduler->current];
        if (currentProcess->status == RUNNING) {
            currentProcess->stack_pointer = prevRSP;
            currentProcess->status = READY;
        }
    }

    process_t *next = getNextProcess();
    if (next == NULL) {
        return prevRSP;
    }
    scheduler->current = next->pid;
    uint64_t nextRSP = next->stack_pointer;
    next->status = RUNNING;

    return (void *) nextRSP;

}


int64_t addProcess(entry_point_t main, char** argv, char* name, uint8_t unkillable, int* fileDescriptors) {
    if (scheduler == NULL || scheduler->size >= MAX_PROCESSES || fileDescriptors == NULL) {
        return NO_PID;
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

    uint16_t parentPid = (scheduler->current == NO_PID) ? scheduler->current : NO_PID;

    process_t *newProcess = createProcess(
        pid,
        parentPid,
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


void yield(void) {

}


uint16_t getPid(void) {

}


int32_t killProcess(uint16_t pid) {

}


int32_t killCurrentProcess(void) {

}


int blockProcess(uint16_t pid) {

}


int unblockProcess(uint16_t pid) {

}


int setPriority(uint16_t pid, uint8_t newPriority) {

}


process_info_t* ps(void) {

}


void myExit(int64_t retValue) {

}


int64_t waitPid(uint32_t pid) {

}


void getFds(int* fds) {

}


int sleepBlock(uint16_t pid, uint8_t time) {

}


void killForegroundProcess(void) {

}


void updateStdinWait(uint8_t value) {

}


static int initMain(int argc, char **argv) {
    char **shellArgs = { NULL };
    int fds[2] = { STDIN, STDOUT };

    int shellPid = addProcess((entry_point_t) SHELL_ADDRESS, shellArgs, "shell", 1, fds);
    setPriority(shellPid, MAX_PRIORITY);

    while (1) {
        for (int i = 0; i < MAX_PROCESSES; i++) {
            if (scheduler->processes[i] != NULL && 
                scheduler->processes[i]->status == TERMINATED && 
                scheduler->processes[i]->parent_pid == INIT_PID) {
                removeProcess(i);
            }
        }
        _hlt();
    }
    return 0;
}

static process_t* getNextProcess(void) {

}

static void removeProcess(uint16_t pid) {

}



