// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "testSynchro.h"

#define SEM_ID "tsem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
	uint64_t aux = *p;
	yield();
	aux += inc;
	*p = aux;
	printf("[test_sync] slowInc: %d -> %d\n", (int)aux - inc, (int)aux);
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

	if (use_sem)
		if (semOpen(SEM_ID, 1) == -1) {
			printf("test_sync: ERROR opening semaphore\n");
			return -1;
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

uint64_t test_sync(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
	
	printf("test_sync: received argc = %d\n", (int)argc);
	
	if (argc != 2) {
		printf("test_sync: ERROR: Invalid number of arguments %d\n", (int)argc);
		printf("test_sync: received argc = %d\n", (int)argc);
		for (uint64_t j = 0; j < argc; j++) {
			printf("test_sync: argv[%d] = '%s'\n", (int)j, argv[j]);
		}
		return -1;
	}
	
	printf("test_sync: launching test with n = %s, use_sem = %s\n", argv[0], argv[1]);
	
	uint64_t pids[2 * TOTAL_PAIR_PROCESSES];
	char *argvDec[] = {argv[0], "-1", argv[1], NULL};
	char *argvInc[] = {argv[0], "1", argv[1], NULL};
	int fds[2] = {STDIN, STDOUT};

	global = 0;

	uint64_t i;
	for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
		printf("[test_sync] Creando proceso decrementador %d\n", i);
		pids[i] = newProcess((EntryPoint) &my_process_inc, argvDec,
									"my_process_dec", 0, fds);
		printf("[test_sync] Creando proceso incrementador %d\n", i);
		pids[i + TOTAL_PAIR_PROCESSES] = newProcess(
			(EntryPoint) &my_process_inc, argvInc, "my_process_inc", 0, fds);
			printf("global = %d\n", global);
	}

	for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
		printf("[test_sync] Esperando proceso decrementador %d (pid = %d)\n", i, pids[i]);
		waitPid(pids[i]);
    printf("[test_sync] Esperando proceso incrementador %d (pid = %d)\n", i, pids[i + TOTAL_PAIR_PROCESSES]);
		waitPid(pids[i + TOTAL_PAIR_PROCESSES]);
		printf("global wait = %d\n", global);
	}
	printf("[test_sync] Final value: %d\n", global);
	return 0;
}