#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdint.h>

#define MEM_SIZE 0x100000 // 1 MiB

typedef struct {
    uint32_t total_mem;
    uint32_t used_mem;
    uint32_t free_mem;
} mem_info_t;

void my_mem_init(void *start, uint32_t size);
void *my_malloc(uint32_t size);
void my_free(void *ptr);
mem_info_t *mem_dump();

#endif // !MEMORY_MANAGER_H
