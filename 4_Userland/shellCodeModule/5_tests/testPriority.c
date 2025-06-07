#include <stdint.h>
#include <testPriority.h>

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

int64_t testPriority() {
  int64_t pids[TOTAL_PROCESSES];
	char *argv[] = {WAIT_STR, NULL};
	int fds[2] = {STDIN, STDOUT};
	uint64_t i;

	for (i = 0; i < TOTAL_PROCESSES; i++) {
		pids[i] = newProcess((EntryPoint) &endlessLoopPrint, argv,
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
		blockProcess(pids[i]);

	printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++)
		nice(pids[i], MEDIUM);

	printf("UNBLOCKING...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++)
		unblockProcess(pids[i]);

	bussyWait(WAIT);
	printf("\nKILLING...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++)
		kill(pids[i]);

  return OK;
}