
#ifndef _TEST_SYNCHRO_H
#define _TEST_SYNCHRO_H

#include <stdint.h>
#include <syscallsInt.h>
#include <stdLib.h>
#include <strLib.h>
#include <testLib.h>
#include <stddef.h>

uint64_t my_process_inc(int argc, char **argv);
uint64_t test_sync(int argc, char **argv);
#define SEM_ID 80
#define TOTAL_PAIR_PROCESSES 2

#endif