#include <phylo.h>

#define MAX_PHILOSOPHERS 10
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define EATING_TIME 500 // milliseconds
#define THINKING_TIME 500 // milliseconds

static char mutexName[] = "mutex_sem";
static char semNames[MAX_PHILOSOPHERS][12]; // s0, s1, ..., s9
static int state[MAX_PHILOSOPHERS] = {THINKING};
static int lastState[MAX_PHILOSOPHERS] = {THINKING};
static uint64_t pidTable[MAX_PHILOSOPHERS] = {0};
static int activePhilosopher[MAX_PHILOSOPHERS] = {0};
static int philosopherCount = 0;

static void getSemName(int i, char *buf);
static int left(int i);
static int right(int i);
void printPhilosophersStatus();
void checkAdjacentEating();
void checkAndPrintStateChange();
void think(int id);
void eat(int id);
void test(int i);
void take_forks(int i);
void put_forks(int i);
uint64_t philosopher(int argc, char **argv);
void addPhilosopher();
void removePhilosopher();
uint64_t phylo(int argc, char **argv);


static void getSemName(int i, char *buf) {
  buf[0] = 's';
  intToString(i, buf + 1, 2);
  buf[3] = '\0';
}

static int left(int i) {
  return (i + philosopherCount - 1) % philosopherCount;
}
static int right(int i) {
  return (i + 1) % philosopherCount;
}

void printPhilosophersStatus() {
  for (int i = 0; i < philosopherCount; i++) {
    if (state[i] == EATING) {
      printf("E ");
    } else {
      printf(". ");
    }
  }
  printf("\n");
}


void checkAdjacentEating() {
  //si es uno solo o cero no hay vecinos
  if(philosopherCount <= 1) {
    return;
  }

  for (int i = 0; i < philosopherCount; i++) {
    int leftNeighbor = left(i);
    int rightNeighbor = right(i);
    
    //Chequear si yo y el vecino de la izquierda estamos comiendo
    if (state[i] == EATING && state[leftNeighbor] == EATING) {
      printf("ESTADO INVALIDO!!!\n", leftNeighbor, i);
    }
    
    //Chequear si yo y el vecino de la derecha estamos comiendo
    if (state[i] == EATING && state[rightNeighbor] == EATING) {
      printf("ESTADO INVALIDO!!!\n", i, rightNeighbor);
    }
  }
}

void checkAndPrintStateChange() {
  int visualChanged = 0;
  int hasE = 0;
  for (int i = 0; i < philosopherCount; i++) {
    // Se ignoran los cambios a o desde HUNGRY para evitar printear estados iguales(explicado en el informe)
    int currentVisual = (state[i] == EATING) ? 1 : 0;
    int lastVisual = (lastState[i] == EATING) ? 1 : 0;
    
    if (currentVisual != lastVisual) {
      visualChanged = 1;
    }
    if(state[i] == EATING) {
      hasE = 1;
    }
    lastState[i] = state[i];
  }
  //se skipea el print si son todos puntos (explicado el informe)
  //pero si es solo 1 me interesa ver si esta pensando
  if (visualChanged && (hasE || philosopherCount == 1)) {
    printPhilosophersStatus();
    //chequea si hay un estado invalido
    checkAdjacentEating();
  }
}

void think(int id) {
  state[id] = THINKING;
  sleep(THINKING_TIME);
}

void eat(int id) {
  state[id] = EATING;
  sleep(EATING_TIME);
}

void test(int i) {
  if (state[i] == HUNGRY && state[left(i)] != EATING && state[right(i)] != EATING) {
    state[i] = EATING;
    checkAndPrintStateChange(); //imprimir despues del cambio de estado
    semPost(semNames[i]);
  }
}

void take_forks(int i) {
  semWait(mutexName);
  state[i] = HUNGRY;
  checkAndPrintStateChange(); //imprimir despues del cambio de estado
  test(i);
  semPost(mutexName);
  semWait(semNames[i]);
}

void put_forks(int i) {
  semWait(mutexName);
  state[i] = THINKING;
  checkAndPrintStateChange(); //imprimir despues del cambio de estado
  test(left(i));
  test(right(i));
  semPost(mutexName);
}

uint64_t philosopher(int argc, char **argv) {
  int id = stringToInt(argv[0]);
  char mySem[12];
  getSemName(id, mySem);
  while (1) {
    think(id);
    take_forks(id);
    eat(id);
    put_forks(id);
  }
  return 0;
}

void addPhilosopher() {
  if(philosopherCount >= MAX_PHILOSOPHERS)
    return;

  int id = philosopherCount;
  char idStr[3];
  char writeFdStr[12];
  intToString(id, idStr, 3);

  char *args[] = {idStr, writeFdStr, NULL};
  int fds[] = {STDIN, STDOUT};

  getSemName(id, semNames[id]);
  semOpen(semNames[id], 0);
  uint64_t pid = newProcess((EntryPoint)&philosopher, args, "philosopher", 0, fds);
  pidTable[id] = pid;
  
  activePhilosopher[id] = 1;
  philosopherCount++;

  printf("Added philosopher %d (pid %d)\n", id, pid);

  checkAndPrintStateChange();
}

void removePhilosopher() {
  if (philosopherCount <= 0)
    return;

  int id = philosopherCount - 1;

  if (activePhilosopher[id]) {
    kill(pidTable[id]);
    waitPid(pidTable[id]);
    semClose(semNames[id]);

    activePhilosopher[id] = 0;
    philosopherCount--;

    printf("Removed philosopher %d\n", id);

    checkAndPrintStateChange();
  }
}

uint64_t phylo(int argc, char **argv) {
  semOpen(mutexName, 1);

  printf("\nCommands:\n[a] Add Philosopher\n[r] Remove Philosopher\n[q] Quit\n");

  while (1) {
    char cmd = getchar();
    if (cmd == 'a') {
      addPhilosopher();
    } else if (cmd == 'r') {
      removePhilosopher();
    } else if (cmd == 'q') {
      break;
    }
  }

  //cierro los semaforos y mato los procesos
  for(int i = philosopherCount - 1; i >= 0; i--) {
    removePhilosopher();
  }

  semClose(mutexName);
  return 0;
}