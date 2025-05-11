#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>
#include "process.h"

#define MAX_PROCESSES 64

typedef struct {
    process_t* processes[MAX_PROCESSES];
    uint8_t front;
    uint8_t rear;
    uint8_t size;
} scheduler_t;

typedef struct {
    uint16_t pid;
    uint8_t priority;
    process_status_t status;
    char name[MAX_NAME_LENGTH];
} process_info_t;


scheduler_t initScheduler(void);


scheduler_t getScheduler(void);


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


process_info_t* ps(void);


void myExit(int64_t retValue);


int64_t waitPid(uint32_t pid);


void getFds(int* fds);


int sleepBlock(uint16_t pid, uint8_t time);


void killForegroundProcess(void);


void updateStdinWait(uint8_t value);


#endif // SCHEDULER_H