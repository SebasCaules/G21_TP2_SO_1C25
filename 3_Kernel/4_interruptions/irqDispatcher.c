#include <time.h>
#include <stdint.h>
#include <keyboard.h>
#include <naiveConsole.h>
#include <videoDriver.h>

static uint64_t int_20(uint64_t rsp);
static void int_21();

static void (*irq_routines[2])() = {
    int_20,
    int_21
};

void irqDispatcher(uint64_t irq) {
    irq_routines[irq]();
	return;
}

uint64_t int_20(uint64_t rsp) {
    rsp = timer_handler(rsp);
    return rsp;
}

void int_21(){
	pressedKey();
}
