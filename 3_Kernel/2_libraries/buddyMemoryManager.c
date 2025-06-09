#ifdef USE_BUDDY

#include "memoryManager.h"
#include <stdint.h>
#include <stddef.h>

// Orden maximo posible, para hacer los arrays
#define MAX_POSSIBLE_ORDER 32
#define MIN_BLOCK_SIZE 32  // Tamanio minimo de bloque (2^5)
#define MIN_ORDER 5        // log2(MIN_BLOCK_SIZE)

typedef struct buddy_block {
    struct buddy_block *next;
    struct buddy_block *prev;
} buddy_block_t;

static buddy_block_t *free_lists[MAX_POSSIBLE_ORDER + 1];// Free lists para cada orden
static uint32_t max_order = 0;  // Orden maximo verdadero
static void *heap_start = NULL;
static uint32_t heap_size = 0;
static mem_info_t mem_info;

//Header de los bloques allocados(se guardan antes de los datos)
typedef struct {
    uint32_t order;     // Orden del bloque
    uint32_t magic;     // Numero magico para validar que no se intente liberar un bloque no asignado
} alloc_header_t;

#define ALLOC_MAGIC 0xDEADBEEF
#define HEADER_SIZE sizeof(alloc_header_t)

static inline uint32_t log2_ceil(uint32_t n) {
    uint32_t order = 0;
    uint32_t size = 1;
    while (size < n) {
        size <<= 1;
        order++;
    }
    return order;
}

static inline uint32_t get_buddy_offset(void *block, uint32_t order) {
    uintptr_t offset = (uintptr_t)block - (uintptr_t)heap_start;
    return offset ^ (1UL << order);
}

static inline void *get_buddy_address(void *block, uint32_t order) {
    uint32_t buddy_offset = get_buddy_offset(block, order);
    return (void *)((uintptr_t)heap_start + buddy_offset);
}

// Aniadir bloque a su lista de libres
static void add_to_free_list(void *block, uint32_t order) {
    buddy_block_t *buddy_block = (buddy_block_t *)block;
    buddy_block->next = free_lists[order];
    buddy_block->prev = NULL;
    
    if (free_lists[order]) {
        free_lists[order]->prev = buddy_block;
    }
    free_lists[order] = buddy_block;
}

// Eliminar bloque de su lista de libres
static void remove_from_free_list(void *block, uint32_t order) {
    buddy_block_t *buddy_block = (buddy_block_t *)block;
    
    if (buddy_block->prev) {
        buddy_block->prev->next = buddy_block->next;
    } else {
        free_lists[order] = buddy_block->next;
    }
    
    if (buddy_block->next) {
        buddy_block->next->prev = buddy_block->prev;
    }
}

// Encontrar el buddy y eliminarlo de la lista de libres
static void *find_and_remove_buddy(void *block, uint32_t order) {
    void *buddy = get_buddy_address(block, order);
    buddy_block_t *curr = free_lists[order];
    
    while (curr) {
        if (curr == buddy) {
            remove_from_free_list(buddy, order);
            return buddy;
        }
        curr = curr->next;
    }
    return NULL;
}

//Dividir un bloque en dos buddies
static void split_block(void *block, uint32_t order) {
    if (order <= MIN_ORDER) return;
    
    uint32_t new_order = order - 1;
    uint32_t block_size = 1UL << new_order;
    
    void *buddy = (void *)((uint8_t *)block + block_size);
    
    add_to_free_list(block, new_order);
    add_to_free_list(buddy, new_order);
}

//Inicializar el memory manager con sistema buddy
void my_mem_init(void *start, uint32_t size) {
    heap_start = start;
    heap_size = size;
    
    //Inicializar la informacion de la memoria
    mem_info.total_mem = size;
    mem_info.used_mem = 0;
    
    //Calcular maximo orden
    max_order = log2_ceil(size);
    if ((1UL << max_order) > size) {
        max_order--;
    }
    
    //Si se pasa del maximo lo limita
    if (max_order > MAX_POSSIBLE_ORDER) {
        max_order = MAX_POSSIBLE_ORDER;
    }
    
    //Inicializar las listas de libres
    for (int i = 0; i <= MAX_POSSIBLE_ORDER; i++) {
        free_lists[i] = NULL;
    }
    
    //Agregar todo el heap como un solo bloque
    add_to_free_list(heap_start, max_order);
}

//Allocar memoria
void *my_malloc(uint32_t size) {
    if (size == 0) return NULL;
    
    //Agregar espacio para el header y calcular orden
    uint32_t total_size = size + HEADER_SIZE;
    uint32_t order = log2_ceil(total_size);
    
    if (order < MIN_ORDER) {
        order = MIN_ORDER;
    }
    
    if (order > max_order) {
        return NULL; //Muy granmde
    }
    
    //Encontrar el primer bloque libre lo suficientemente grande
    uint32_t alloc_order = order;
    while (alloc_order <= max_order && !free_lists[alloc_order]) {
        alloc_order++;
    }
    
    if (alloc_order > max_order) {
        return NULL; //No hay memoria disponible
    }
    
    //Conseguir bloque desde la lista de libres
    void *block = free_lists[alloc_order];
    remove_from_free_list(block, alloc_order);
    
    //Dividir bloque hasta tener el tamanio adecuado
    while (alloc_order > order) {
        alloc_order--;
        split_block(block, alloc_order + 1);
        //Despues del split nuestro bloque es el primero
        remove_from_free_list(block, alloc_order);
    }
    
    //Setup del header
    alloc_header_t *header = (alloc_header_t *)block;
    header->order = order;
    header->magic = ALLOC_MAGIC;
    
    //Update a la informacion de memoria
    mem_info.used_mem += (1UL << order);
    
    //Devuelve el puntero a los datos(despues del header))
    return (void *)((uint8_t *)block + HEADER_SIZE);
}

//Free del bloque y merge con el buddy(si se puede)
void my_free(void *ptr) {
    if (!ptr) return;
    
    //Busco el header del bloque
    alloc_header_t *header = (alloc_header_t *)((uint8_t *)ptr - HEADER_SIZE);
    
    //Valido magic number
    if (header->magic != ALLOC_MAGIC) {
        return; //El puntero es invalido o hubo corrupcion de datos
    }
    
    void *block = (void *)header;
    uint32_t order = header->order;
    
    //Update a la informacion de memoria
    mem_info.used_mem -= (1UL << order);
    
    //Cleareo el header
    header->magic = 0;
    
    //Mergeo con buddies lo mas que pueda
    while (order < max_order) {
        void *buddy = find_and_remove_buddy(block, order);
        if (!buddy) {
            break; //Buddy en uso, no puedo mergear
        }
        
        //Mergeo con buddy, el primer bloque pasa a ser el nuevo bloque
        if (buddy < block) {
            block = buddy;
        }
        order++;
    }
    
    //Agrego el bloque(si mergee agrego ese) a la lista de libres
    add_to_free_list(block, order);
}

//Da la informacion de la memoria
mem_info_t *mem_dump() {
    mem_info.free_mem = mem_info.total_mem - mem_info.used_mem;
    return &mem_info;
}
#endif // USE_BUDDY

/*
Explicación Visual del Buddy System:

ESTRUCTURA DE MEMORIA:
Heap dividido en bloques de tamaños potencia de 2:

| 32B | 32B | 64B      | 128B          | 256B                    |
|order|order|  order   |    order      |       order             |
|  5  |  5  |    6     |      7        |         8               |

FREE LISTS (Array de listas enlazadas):
free_lists[5] -> [Bloque32B] <-> [Bloque32B] <-> NULL
free_lists[6] -> [Bloque64B] <-> NULL  
free_lists[7] -> NULL (no hay bloques de 128B libres)
free_lists[8] -> [Bloque256B] <-> NULL

Header contiene:
- order: orden del bloque (para liberación eficiente)
- magic: 0xDEADBEEF (validación de corrupción)

MAGIC:
El uso de magic number es algo que se recomienda usar para detectar corrupcion
o que se haya pasado mal un puntero a una rutina
La idea fue sacada del apartado "Linux magic numbers" de la documentacion del kernel de Linux:
https://www.kernel.org/doc/html/v4.14/process/magic-number.html
*/