// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "testProcess.h"
#include <stddef.h>
#include <colors.h>

int64_t testProcess(uint64_t argc, char *argv[]) {
	uint8_t rq;
	uint8_t alive = 0;
	uint8_t action;
	uint64_t max_processes;
	char *argvAux[] = {0, NULL};
	int fds[2] = {STDIN, STDOUT};

	if (argc != 1) {
		printf("test_processes: ERROR: Invalid number of arguments\n");
		return -1;
	}
	int flag;
	if ((max_processes = satoi(argv[0], &flag)) <= 0) {
		printf("test_processes: ERROR: Invalid argument\n");
		return -1;
	}

	if (max_processes >= MAX_PROCESSES) {
		printf("test_processes: ERROR: Too many processes requested\n");
		return ERROR;
	}

	printf("Este test tiene 2 sleeps de 1 y 2 segundos luego de los prints\n");

	p_rq p_rqs[max_processes];

	while (1) {
		// Create max_processes processes
		for (rq = 0; rq < max_processes; rq++) {
			p_rqs[rq].pid = newProcess((EntryPoint) &endlessLoop, argvAux, "endless_loop", 0, fds);
			if (p_rqs[rq].pid == -1) {
				printf("test_processes: ERROR creating process\n");
				return -1;
			}
			else {
				p_rqs[rq].state = RUNNING;
				alive++;
			}
		}

		// Randomly kills, blocks or unblocks processes until every one has been killed
		while (alive > 0) {
			printf("Procesos vivos:\n");
			uint8_t shown = 0;
			for (uint8_t i = 0; i < max_processes; i++) {
				if (p_rqs[i].state != TERMINATED) {
					int fdIdx = (i + 1) % FDS_COUNT;
					if (fdIdx == 0)
						fdIdx++;
					char letter = 'a' + i;
					if (letter > 'z') {
						letter = 'A' + (i - 26);
					}
					fdprintf(fdIdx, "[%c] PID: %d  ", letter, p_rqs[i].pid);
				}
			}
			printf("\n");
			sleep(500);

			for (rq = 0; rq < max_processes; rq++) {
				action = getUniform(100) % 2;

				switch (action) {
					case 0:
						if (p_rqs[rq].state == RUNNING ||
							p_rqs[rq].state == BLOCKED) {
							if (kill(p_rqs[rq].pid) == -1) {
								printf("test_processes: ERROR killing process\n");
								return -1;
							}
							p_rqs[rq].state = TERMINATED;
							alive--;
						}
						break;
					case 1:
						if (p_rqs[rq].state == RUNNING) {
							if (blockProcess(p_rqs[rq].pid) == -1) {
								printf("test_processes: ERROR blocking process\n");
								return -1;
							}
							p_rqs[rq].state = BLOCKED;
						}
						break;
				}
			}

			// Randomly unblocks processes
			for (rq = 0; rq < max_processes; rq++)
				if (p_rqs[rq].state == BLOCKED && getUniform(100) % 2) {
					if (unblockProcess(p_rqs[rq].pid) == -1) {
						printf("test_processes: ERROR unblocking process\n");
						return -1;
					}
					p_rqs[rq].state = RUNNING;
				}
		}
	}
}