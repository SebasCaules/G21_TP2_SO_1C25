#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdint.h>

// Inicializa el administrador de memoria
// Recibe la dirección donde termina el kernel (kernel_end_address)
void init_memory_manager(uint64_t kernel_end_address);

// Asigna una página de 4KB
void *alloc_page(void);

// Libera una página
void free_page(void *addr);

#endif