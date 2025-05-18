

#ifndef TEST_PROCESS_H
#define TEST_PROCESS_H

#include <stdint.h>
#include <testLib.h>
#include <syscallsInt.h>
#include <stdLib.h>


enum State { RUNNING, BLOCKED, KILLED };

typedef struct P_rq {
  int32_t pid;
  enum State state;
} p_rq;

int64_t testProcess(uint64_t argc, char *argv[]);

#endif