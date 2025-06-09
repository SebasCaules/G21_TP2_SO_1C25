// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "syscallsInt.h"
#include "testLib.h"
#include <stdint.h>
#include <stdio.h>

#define MINOR_WAIT 1000000
#define WAIT 1000000000
#define WAIT_STR "30000000"

#define TOTAL_PROCESSES 4

#define LOWEST 1
#define MEDIUM_LOW 2
#define MEDIUM_HIGH 3
#define HIGHEST 4

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM_LOW, MEDIUM_HIGH, HIGHEST};

void testPriority() {
	int64_t pids[TOTAL_PROCESSES];
	char *argv[] = {WAIT_STR, NULL};
	int fds[2] = {STDIN, STDOUT};
	uint64_t i;

	for (i = 0; i < TOTAL_PROCESSES; i++) {
		pids[i] = sys_new_process((EntryPoint) &endlessLoopPrint, argv,
									"endless_loop_print", 0, fds);
		if (pids[i] < 0) {
			printf("test_prio: error creating process %d\n", (int) i);
		}
	}

	bussyWait(WAIT);
	printf("\nCHANGING PRIORITIES...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++)
		nice(pids[i], prio[i]);

	bussyWait(WAIT);
	printf("\nBLOCKING...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++)
		sys_block_process(pids[i]);

	printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++)
		nice(pids[i], MEDIUM_LOW);

	printf("UNBLOCKING...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++)
		sys_unblock_process(pids[i]);

	bussyWait(WAIT);
	printf("\nKILLING...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++)
		sys_kill_process(pids[i]);
}