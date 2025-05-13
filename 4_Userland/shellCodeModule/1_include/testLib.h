
#ifndef TEST_UTIL_H
#define TEST_UTIL_H
#include <stdint.h>
#include <syscallsInt.h>
#include <stdLib.h>

uint32_t getUInt();
uint32_t getUniform ( uint32_t max );
uint8_t memcheck ( void * start, uint8_t value, uint32_t size );
void bussyWait ( uint64_t n );
void endlessLoop();
void endlessLoopPrint ( uint64_t wait );
void endlessLoopPrintMain ( char ** argv, uint64_t argc );

#endif