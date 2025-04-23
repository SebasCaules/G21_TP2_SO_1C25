#include <time.h>
#include <stdint.h>
#include <keyboard.h>
#include <naiveConsole.h>
#include <videoDriver.h>

static void int_20();
static void int_21();

static void (*irq_routines[2])() = {
    int_20,
    int_21
};

void irqDispatcher(uint64_t irq) {
    irq_routines[irq]();
	return;
}

void int_20() {
	timer_handler();
}

void int_21(){
	pressedKey();
}
