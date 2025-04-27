#include "memoryManager.h"

#define PAGE_SIZE 4096 // 4 KB por página

// Cambiá esto si tu memoria es más chica o más grande (por ahora ponemos 128MB)
#define TOTAL_MEMORY (128 * 1024 * 1024)
#define TOTAL_PAGES (TOTAL_MEMORY / PAGE_SIZE)

// Bitmap de páginas: 0 = libre, 1 = ocupado
static uint8_t page_bitmap[TOTAL_PAGES];

// Dirección física donde arranca la memoria libre
static uint64_t free_memory_start = 0;

// Inicializa el memory manager
void init_memory_manager(uint64_t kernel_end_address) {
    for (uint64_t i = 0; i < TOTAL_PAGES; i++) {
        page_bitmap[i] = 0; // Todo libre inicialmente
    }

    free_memory_start = kernel_end_address;

    uint64_t pages_reserved = (kernel_end_address + PAGE_SIZE - 1) / PAGE_SIZE;
    for (uint64_t i = 0; i < pages_reserved; i++) {
        page_bitmap[i] = 1; // Marcar páginas usadas por el kernel
    }
}

// Asigna una página libre y la marca como ocupada
void *alloc_page() {
    for (uint64_t i = 0; i < TOTAL_PAGES; i++) {
        if (page_bitmap[i] == 0) {
            page_bitmap[i] = 1;
            return (void *)(i * PAGE_SIZE);
        }
    }
    return 0; // No hay páginas disponibles
}

// Libera una página (la marca como libre)
void free_page(void *addr) {
    uint64_t page = ((uint64_t)addr) / PAGE_SIZE;
    if (page < TOTAL_PAGES) {
        page_bitmap[page] = 0;
    }
}