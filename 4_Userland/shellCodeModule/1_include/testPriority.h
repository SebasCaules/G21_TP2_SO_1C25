
#ifndef TEST_PRIORITY_H
#define TEST_PRIORITY_H

#include <stdint.h>
#include <testLib.h>
#include <syscallsInt.h>
#include <stdLib.h>

#define MINOR_WAIT 1000000
#define WAIT 10000000 
#define WAIT_STR "30000000"

#define TOTAL_PROCESSES 5
#define LOWEST 0  
#define MEDIUM 1  
#define HIGHEST 2 

int64_t testPriority();


#endif