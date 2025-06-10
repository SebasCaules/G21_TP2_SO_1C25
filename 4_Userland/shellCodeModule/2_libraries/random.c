// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <random.h>
#include <stdint.h>

unsigned int seed;

void updateSeed(unsigned int newSeed) {
    seed = newSeed;
}

unsigned int getRandomInt(int min, int max) {
    seed = sys_tick();
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return min + (seed % (max - min + 1));
}
