#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdint.h>

void my_mem_init(void *start, uint32_t size);
void *my_malloc(uint32_t size);
void my_free(void *ptr);

#endif // !MEMORY_MANAGER_H
