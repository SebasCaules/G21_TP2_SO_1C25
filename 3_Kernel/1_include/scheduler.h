#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>
#include "process.h"

#define MAX_PROCESSES 64
#define MAX_PRIORITY 4
#define MIN_PRIORITY 1

typedef struct {
    char name[MAX_NAME_LENGTH];
    uint16_t pid;
    int16_t ppid;
    uint8_t priority;
    void *stackBase;
    void *stackPointer;
    bool foreground;
    process_status_t status;
    uint64_t cpuTicks; // para calcular el %CPU (eventualmente)
} process_info_t;

typedef struct scheduler_t * schedulerADT;

schedulerADT initScheduler(void);


schedulerADT getScheduler(void);


void* schedule(void* prevRSP);


int64_t addProcess(entry_point_t main, char** argv, char* name, 
    uint8_t unkillable, int* fileDescriptors);


void destroyScheduler(void);


void yield(void);


uint16_t getPid(void);


int32_t killProcess(uint16_t pid);


int32_t killCurrentProcess(void);


int blockProcess(uint16_t pid);


int unblockProcess(uint16_t pid);


int setPriority(uint16_t pid, uint8_t newPriority);


process_info_t* processStatus(void);


void myExit(int64_t retValue);


int64_t waitPid(uint32_t pid);


void getFds(int* fds);


int sleepBlock(uint16_t pid, uint8_t time);


void killForegroundProcess(void);


void updateStdinWait(uint8_t value);


#endif // SCHEDULER_H