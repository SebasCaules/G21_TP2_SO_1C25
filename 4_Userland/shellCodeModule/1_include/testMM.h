
#ifndef TEST_MM_H
#define TEST_MM_H

#include <stdint.h>
#include <testLib.h>
#include <syscallsInt.h>
#include <stdLib.h>


#define MAX_BLOCKS 20

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

int64_t testMM(uint64_t argc, char * argv[]);


#endif