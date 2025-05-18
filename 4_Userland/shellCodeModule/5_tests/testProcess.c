#include <testProcess.h>

int64_t testProcess(uint64_t argc, char *argv[]) {
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;
  char *argvAux[] = {0};
  int fds[2] = { STDIN, STDOUT };

  if (argc != 1) {
    printf("test_processes: Invalid number of arguments. Expected 1, got %lu\n", argc);
    return -1;
  }

  int64_t flag;
  max_processes = satoi(argv[0], &flag);
  if (!flag || max_processes <= 0) {
    printf("test_processes: Invalid max_processes value: %lu\n", max_processes);
    return -1;
  }

  p_rq p_rqs[max_processes];
  
  while (1) {
    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      p_rqs[rq].pid = newProcess((EntryPoint) &endlessLoop, argvAux, "endless_loop", 0, fds);
      printf("test_processes: Created process %d\n", p_rqs[rq].pid);

      if (p_rqs[rq].pid < 0) {
        printf("test_processes: ERROR creating process: %d\n", p_rqs[rq].pid);
        return -1;
      } else {
        p_rqs[rq].state = RUNNING;
        alive++;
        printf("test_processes: Process %d is now RUNNING. Alive count: %d\n", p_rqs[rq].pid, alive);
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0) {
      for (rq = 0; rq < max_processes; rq++) {
        action = getUniform(100) % 2;

        switch (action) {
          case 0: // Kill process
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
              printf("test_processes: Attempting to kill process %d\n", p_rqs[rq].pid);
              if (kill(p_rqs[rq].pid) == -1) {
                printf("test_processes: ERROR killing process %d\n", p_rqs[rq].pid);
                return -1;
              }
              p_rqs[rq].state = KILLED;
              alive--;
              printf("test_processes: Process %d is now KILLED. Alive count: %d\n", p_rqs[rq].pid, alive);
            }
            break;

          case 1: // Block process
            if (p_rqs[rq].state == RUNNING) {
              printf("test_processes: Attempting to block process %d\n", p_rqs[rq].pid);
              int toReturn = blockProcess(p_rqs[rq].pid);
              if (toReturn < 0) {
                printf("test_processes: ERROR blocking process %d\n", p_rqs[rq].pid);
                return -1;
              }
              p_rqs[rq].state = BLOCKED;
              printf("test_processes: Process %d is now BLOCKED\n", p_rqs[rq].pid);
            }
            break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++) {
        if (p_rqs[rq].state == BLOCKED && getUniform(100) % 2) {
          printf("test_processes: Attempting to unblock process %d\n", p_rqs[rq].pid);
          if (unblockProcess(p_rqs[rq].pid) == -1) {
            printf("test_processes: ERROR unblocking process %d\n", p_rqs[rq].pid);
            return -1;
          }
          p_rqs[rq].state = RUNNING;
          printf("test_processes: Process %d is now RUNNING\n", p_rqs[rq].pid);
        }
      }
    }
  }
}