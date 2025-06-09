#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoryManager.h"

#define MAX_BLOCKS 128

uint8_t memcheck ( void * start, uint8_t value, uint32_t size ) {
	uint8_t * p = ( uint8_t * ) start;
	uint32_t i;

	for ( i = 0; i < size; i++, p++ )
		if ( *p != value )
			return 0;

	return 1;
}

// Simple replacement for atoi that returns -1 on invalid input
int satoi(const char *str) {
  if (str == NULL) return -1;
  int result = 0;
  while (*str) {
    if (*str < '0' || *str > '9') return -1;
    result = result * 10 + (*str - '0');
    str++;
  }
  return result;
}

// Returns a uniform random number in the range [0, max]
int getUniformMM(int max) {
  return rand() % (max + 1);
}

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;

uint64_t test_mm(uint64_t argc, char *argv[]);

int main(void) {
	char *args[] = {"131072"};
	return test_mm(1, args);
}

uint64_t test_mm(uint64_t argc, char *argv[]) {

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;

  if (argc != 1)
    return -1;

  if ((max_memory = satoi(argv[0])) <= 0)
    return -1;

  printf("Running test_mm with max_memory = %llu\n", max_memory);

  void *start = malloc(max_memory);
  if (!start) {
    printf("Failed to initialize memory pool\n");
    return -1;
  }

  printf("Memory pool initialized at address %p with size %llu bytes\n", start, max_memory);

  while (1) {
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {
      mm_rqs[rq].size = getUniformMM(max_memory - total - 1) + 1;
      mm_rqs[rq].address = malloc(mm_rqs[rq].size);

      if (mm_rqs[rq].address) {
        printf("Allocated block %u: %u bytes at %p\n", rq, mm_rqs[rq].size, mm_rqs[rq].address);
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);

    printf("All blocks set\n");

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          printf("test_mm ERROR\n");
          return -1;
        }

    printf("All blocks passed verification\n");

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        free(mm_rqs[i].address);

    printf("All blocks freed\n");
    sleep(1);
  }
}