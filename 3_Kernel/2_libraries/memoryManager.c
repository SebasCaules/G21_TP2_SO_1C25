// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef USE_BUDDY

#include "memoryManager.h"
#include <stdint.h>
#include <stddef.h>

typedef struct block_header {
	uint32_t size;
	uint8_t free;
	struct block_header *next;
} block_header_t;

#define ALIGN4(x) (((((x) - 1) >> 2) << 2) + 4)
#define BLOCK_HEADER_SIZE sizeof(block_header_t)

static void *heap_start = NULL;
static uint32_t heap_size = 0;
static block_header_t *free_list = NULL;
static mem_info_t mem_info;

void my_mem_init(void *start, uint32_t size) {
    heap_start = start;
    heap_size = size;

    mem_info.total_mem = size;
    mem_info.used_mem = 0;

    free_list = (block_header_t *)start;
    free_list->size = size - BLOCK_HEADER_SIZE; // solo la parte de datos
    free_list->free = 1;
    free_list->next = NULL;
}

void *my_malloc(uint32_t size) {
    size = ALIGN4(size);

    block_header_t *curr = free_list;

    while (curr != NULL) {
        if (curr->free && curr->size >= size) {
            // Si el bloque es suficientemente grande para dividir
            if (curr->size >= size + BLOCK_HEADER_SIZE + 4) {
                // Dividimos el bloque en uno asignado y otro libre
                block_header_t *new_block = (block_header_t *)((uint8_t *)curr + BLOCK_HEADER_SIZE + size);
                new_block->size = curr->size - size - BLOCK_HEADER_SIZE;
                new_block->free = 1;
                new_block->next = curr->next;

                curr->size = size;
                curr->next = new_block;
            }

            curr->free = 0;
            mem_info.used_mem += curr->size + BLOCK_HEADER_SIZE;

            // Retornamos la dirección después de la cabecera (donde empieza el bloque útil)
            return (void *)((uint8_t *)curr + BLOCK_HEADER_SIZE);
        }

        curr = curr->next;
    }

    return NULL;
}

void my_free(void *ptr) {
    if (!ptr) return;

    block_header_t *block = (block_header_t *)((uint8_t *)ptr - BLOCK_HEADER_SIZE);
    block->free = 1;

    mem_info.used_mem -= block->size + BLOCK_HEADER_SIZE;

    block_header_t *curr = free_list;
    while (curr != NULL && curr->next != NULL) {
        if (curr->free && curr->next->free &&
            (uint8_t *)curr + curr->size + BLOCK_HEADER_SIZE == (uint8_t *)curr->next) {
            curr->size += BLOCK_HEADER_SIZE + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}

mem_info_t *mem_dump() {
    mem_info.free_mem = mem_info.total_mem - mem_info.used_mem;
    return &mem_info;
}
#endif // !USE_BUDDY

/*
Explicacion visual:
| Header | Datos... | Header | Datos... | Header | Datos... |
^ heap_start

Header contiene:
Tamaño de datos del bloque
Si está libre
Siguiente bloque
*/

// Algoritmo heap 4: https://freertos.org/Documentation/02-Kernel/02-Kernel-features/09-Memory-management/01-Memory-management#heap_4c

