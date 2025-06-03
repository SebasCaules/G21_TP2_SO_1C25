

#ifndef TEST_PROCESS_H
#define TEST_PROCESS_H

#include <stdint.h>
#include <testLib.h>
#include <syscallsInt.h>
#include <stdLib.h>


typedef struct P_rq {
  int32_t pid;
  process_status_t state;
} p_rq;

int64_t testProcess(uint64_t argc, char *argv[]);

#endif