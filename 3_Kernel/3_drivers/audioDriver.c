// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
// https://wiki.osdev.org/PC_Speaker
#include <audioDriver.h>

int inb(int port);
void outb(int port, int value);

static void play_sound(uint32_t nFrequence) {
	uint32_t Div;
	uint8_t tmp;
    
        //Set the PIT to the desired frequency
	Div = 1193180 / nFrequence;
	outb(0x43, 0xb6);
	outb(0x42, (uint8_t) (Div) );
	outb(0x42, (uint8_t) (Div >> 8));

        //And play the sound using the PC speaker
	tmp = inb(0x61);
	if (tmp != (tmp | 3)) {
		outb(0x61, tmp | 3);
	}
}

//make it shut up
static void nosound() {
	uint8_t tmp = inb(0x61) & 0xFC;

	outb(0x61, tmp);
}

//Make a beep
void beep(uint64_t freq, uint64_t time) {
	play_sound(freq);
	sys_sleep(time);
	nosound();
}
