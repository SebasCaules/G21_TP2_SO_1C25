// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "testSynchro.h"

#define SEM_ID "tsem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
	uint64_t aux = *p;
	yield();
	aux += inc;
	*p = aux;
	printf("slowInc: %d -> %d\n", (int)aux - inc, (int)aux);
}

uint64_t my_process_inc(int argc, char **argv) {
	uint64_t n;
	int8_t inc;
	int8_t use_sem;
	int64_t flag;

	if (argc != 3) {
		fdprintf(STDERR, "ERROR: Invalid number of arguments\n");
		return ERROR;
	}

	if ((n = satoi(argv[0], &flag)) <= 0 || !flag) {
		fdprintf(STDERR, "ERROR: Invalid argument\n");
		return ERROR;
	}
	if ((inc = satoi(argv[1], &flag)) == 0 || !flag) {
		fdprintf(STDERR, "ERROR: Invalid argument\n");
		return ERROR;
	}
	if ((use_sem = satoi(argv[2], &flag)) < 0 || !flag) {
		fdprintf(STDERR, "ERROR: Invalid argument\n");
		return ERROR;
	}

	if (use_sem)
		if (semOpen(SEM_ID, 1) == -1) {
			fdprintf(STDERR, "ERROR opening semaphore\n");
			return ERROR;
		}

	uint64_t i;
	for (i = 0; i < n; i++) {
		if (use_sem) {
			semWait(SEM_ID);
		}
		slowInc(&global, inc);
		if (use_sem) {
			semPost(SEM_ID);
		}
	}

	if (use_sem) {
		semClose(SEM_ID);
	}

	return 0;
}

uint64_t test_sync(int argc, char **argv) { //{n, use_sem, 0}
	
	if (argc != 2) {
		fdprintf(STDERR, "ERROR: Invalid argc = %d\n", (int)argc);
		return ERROR;
	}
	
	fdprintf(STDOUT, "starting with n = %s, use_sem = %s\n", argv[0], argv[1]);
	
	uint64_t pids[2 * TOTAL_PAIR_PROCESSES];
	char *argvDec[] = {argv[0], "-1", argv[1], NULL};
	char *argvInc[] = {argv[0], "1", argv[1], NULL};
	int fds[2] = {STDIN, STDOUT};

	global = 0;

	uint64_t i;
	for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
		pids[i] = newProcess((EntryPoint) &my_process_inc, argvDec,
									"my_process_dec", 0, fds);
		fdprintf(BLUE, "proc_dec %d PID: %d\n", i, pids[i]);
		pids[i + TOTAL_PAIR_PROCESSES] = newProcess(
			(EntryPoint) &my_process_inc, argvInc, "my_process_inc", 0, fds);
		fdprintf(MAGENTA, "proc_inc %d PID: %d\n", i, pids[i + TOTAL_PAIR_PROCESSES]);
	}

	for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
		fdprintf(ORANGE, "wait dec %d (pid %d)\n", i, pids[i]);
		waitPid(pids[i]);
		fdprintf(YELLOW, "wait inc %d (pid %d)\n", i, pids[i + TOTAL_PAIR_PROCESSES]);
		waitPid(pids[i + TOTAL_PAIR_PROCESSES]);
	}
	fdprintf(STDMARK, "FINAL = %d\n", global);
	return 0;
}