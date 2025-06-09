
#ifndef TEST_UTIL_H
#define TEST_UTIL_H
#include <stdint.h>
#include <syscallsInt.h>
#include "colors.h"

uint32_t getUInt();
uint32_t getUniform (uint32_t max);
uint8_t memcheck (void * start, uint8_t value, uint32_t size);
void bussyWait (uint64_t n);
void endlessLoop();
int endlessLoopPrint(int argc, char *argv[]);


#endif