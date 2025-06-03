#ifndef MEMORY_MANAGER_INC_H
#define MEMORY_MANAGER_INC_H

typedef struct {
    uint32_t total_mem;
    uint32_t used_mem;
    uint32_t free_mem;
} mem_info_t;

#endif // MEMORY_MANAGER_INC_H