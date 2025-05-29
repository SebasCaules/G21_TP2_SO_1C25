// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "testSynchro.h"

#define SEM_ID "tsy_sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
	uint64_t aux = *p;
	yield();
	aux += inc;
	*p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[]) {
	uint64_t n;
	int8_t inc;
	int8_t use_sem;
	int64_t flag;

	if (argc != 3) {
		printf("test_sync: ERROR: Invalid number of arguments\n");
		return -1;
	}

	if ((n = satoi(argv[0], &flag)) <= 0 || !flag) {
		printf("test_sync: ERROR: Invalid argument\n");
		return -1;
	}
	if ((inc = satoi(argv[1], &flag)) == 0 || !flag) {
		printf("test_sync: ERROR: Invalid argument\n");
		return -1;
	}
	if ((use_sem = satoi(argv[2], &flag)) < 0 || !flag) {
		printf("test_sync: ERROR: Invalid argument\n");
		return -1;
	}

	printf("Process %d: starting with n = %d, inc = %d, use_sem = %d\n", getPid(), (int)n, (int)inc, (int)use_sem);
	printf("use_sem %s\n", use_sem);

	if (use_sem)
		if (semOpen(SEM_ID, 1) == -1) {
			printf("test_sync: ERROR opening semaphore\n");
			return -1;
		}

	uint64_t i;
	for (i = 0; i < n; i++) {
		printf("Process %d: iteration %d, current value = %d\n", getPid(), (int)i, (int)global);
		if (use_sem) {
			semWait(SEM_ID);
		}
		slowInc(&global, inc);
		printf("Process %d: iteration %d, current value = %d\n", getPid(), (int)i, (int)global);
		if (use_sem) {
			semPost(SEM_ID);
		}
	}

	if (use_sem) {
		semClose(SEM_ID);
	}

	return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
	uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

	printf("test_sync: received argc = %d\n", (int)argc);
	for (uint64_t j = 0; j < argc; j++) {
		printf("test_sync: argv[%d] = '%s'\n", (int)j, argv[j]);
	}

	if (argc != 2) {
		printf("test_sync: ERROR: Invalid number of arguments %d\n", (int)argc);
		printf("test_sync: received argc = %d\n", (int)argc);
		for (uint64_t j = 0; j < argc; j++) {
			printf("test_sync: argv[%d] = '%s'\n", (int)j, argv[j]);
		}
		return -1;
	}

	printf("test_sync: launching test with n = %s, use_sem = %s\n", argv[0], argv[1]);

	char *argvDec[] = {argv[0], "-1", argv[1], NULL};
	char *argvInc[] = {argv[0], "1", argv[1], NULL};
	int fds[2] = {STDIN, STDOUT};

	global = 0;

	uint64_t i;
	for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
		printf("test_sync: creating my_process_dec (i = %d)\n", (int)i);
		pids[i] = newProcess((EntryPoint) &my_process_inc, argvDec,
									"my_process_dec", 0, fds);
		printf("test_sync: creating my_process_inc (i = %d)\n", (int)i);
		pids[i + TOTAL_PAIR_PROCESSES] = newProcess(
			(EntryPoint) &my_process_inc, argvInc, "my_process_inc", 0, fds);
	}

	for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
		waitPid(pids[i]);
		waitPid(pids[i + TOTAL_PAIR_PROCESSES]);
	}

	printf("test_sync: all processes finished.\n");

	printf("test_sync: FINAL value of global after all processes: %d\n", (int)global);
	printf("test_sync: test complete — expected final value is 0 if semaphores work.\n");

	return 0;
}