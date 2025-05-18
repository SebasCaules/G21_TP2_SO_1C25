
#ifndef TEST_PRIORITY_H
#define TEST_PRIORITY_H

#include <stdint.h>
#include <testLib.h>
#include <syscallsInt.h>
#include <stdLib.h>

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 10000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 0  // TODO: Change as required
#define MEDIUM 1  // TODO: Change as required
#define HIGHEST 2 // TODO: Change as required

int64_t testPriority();


#endif