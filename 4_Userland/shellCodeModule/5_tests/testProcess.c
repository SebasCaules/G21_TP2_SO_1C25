// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "testProcess.h"
#include <stddef.h>

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

	p_rq p_rqs[max_processes];

	while (1) {
		// Create max_processes processes
		printf("Sea crean los procesos\n");
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

		// Randomly kills, blocks or unblocks processes until every one has been
		// killed
		printf("Se matan los procesos\n");
		while (alive > 0) {
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
							p_rqs[rq].state = KILLED;
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