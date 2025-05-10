#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#define SCHEDULER_ADDRESS ((void *) 0x60000)        // Scheduler data
#define SEMAPHORE_ADDRESS ((void *) 0x70000)        // Semaphore data
#define START_FREE_MEM ((void *) 0xF00000)          // Heap start address
#define SHELL_ADDRESS ((void *) 0x400000)           // User shell code
#define MEMORY_MANAGER_ADDRESS ((void *) 0x600000)  // Memory manager data

#endif // !MEMORY_MAP_H
