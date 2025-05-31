#include <stdint.h>
#include "videoDriver.h"
#include "keyboard.h"
#include "synchro.h"
#include "scheduler.h"

#define NKEYS 58
#define BUFFER_SIZE 256
#define MAX_PRESS_CODE 0x70
#define REGS_AMOUNT 17

// Define de teclas especiales:
#define ESC 0x01
#define ENTER 0x1C
#define BACKSPACE 0x0E
#define TAB 0x0F
#define L_SHIFT_PRESS 0x2A
#define L_SHIFT_RELEASE 0xAA
#define R_SHIFT_PRESS 0x36
#define R_SHIFT_RELEASE 0xB6
#define CAPS_LOCK_PRESS 0x3A
#define ALT_PRESS 0x3B
#define ALT_RELEASE 0xB8

#define UP_ARROW 0x48
#define DOWN_ARROW 0x50
#define LEFT_ARROW 0x4B
#define RIGHT_ARROW 0x4D

#define CTRL_PRESS 0x1D
#define CTRL_RELEASE 0x9D

#define KEYBOARD_SEM_NAME "keyboard_sem"
#define EOF -1

extern void _updateRegisters();
extern uint64_t * _getRegisters();
static void updateRegisters();
static void sendEOFToSTDIN();

static uint8_t regShotFlag = 0;
static char eofFlag = 0;

static volatile uint64_t registers[REGS_AMOUNT];

static unsigned char keyValues[NKEYS][2] = {
	{0, 0}, {27, 27}, {'1', '!'}, {'2', '@'}, {'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '^'}, {'7', '&'},
	{'8', '*'}, {'9', '('}, {'0', ')'}, {'-', '_'}, {'=', '+'}, {'\b', '\b'}, {9, 9}, {'q', 'Q'}, {'w', 'W'}, 
	{'e', 'E'}, {'r', 'R'}, {'t', 'T'}, {'y', 'Y'}, {'u', 'U'}, {'i', 'I'}, {'o', 'O'}, {'p', 'P'}, {'[', '{'},
	{']', '}'}, {'\n', '\n'}, {0, 0}, {'a', 'A'}, {'s', 'S'}, {'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'},
	{'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {';', ':'}, {39, 34}, {'`', '~'}, {0, 0}, {'\\', '|'}, {'z', 'Z'},
	{'x', 'X'}, {'c', 'C'}, {'v', 'V'}, {'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',', '<'}, {'.', '>'}, {'/', '?'},
	{0, 0}, {0, 0}, {0, 0}, {' ', ' '},
};

static char isSpecialKey(int key) {
    switch(key) {
        case L_SHIFT_PRESS:
        case R_SHIFT_PRESS:
        case CAPS_LOCK_PRESS:
        case ALT_PRESS:
        case ESC:
        case UP_ARROW:
        case DOWN_ARROW:
        case LEFT_ARROW:
        case RIGHT_ARROW:
            return 1;
        default:
            return 0;
    }
}

int initKeyboardDriver() {
	if (semOpen(KEYBOARD_SEM_NAME, 0) == -1) {
		return -1;
	}
	return 0;
}

static char buffer[BUFFER_SIZE];
static int current = 0;
static int nextToRead = 0;

int shiftFlag = 0;
int capsLockFlag = 0;
int ctrlFlag = 0;

void pressedKey(){
    int c = getPressedKey();

    switch (c) {
    case L_SHIFT_PRESS:
    case R_SHIFT_PRESS:
        shiftFlag = 1;
        break;
    case L_SHIFT_RELEASE:
    case R_SHIFT_RELEASE:
        shiftFlag = 0;
        return;
    case CAPS_LOCK_PRESS:
        capsLockFlag = !capsLockFlag;
        break;
	case CTRL_PRESS:
			ctrlFlag = 1;
			break;
	case CTRL_RELEASE:
			ctrlFlag = 0;
			break;
    }

    if (c <= MAX_PRESS_CODE) {
        if(isSpecialKey(c)) {
            buffer[current++] = c;
			semPost(KEYBOARD_SEM_NAME);
        } else {
            int index = shiftFlag;
            //Chequea que la tecla apretada sea una letra, si lo es y el capslock esta activado se van a poner en mayus
            if(keyValues[c][0] >= 'a' && keyValues[c][0] <= 'z'){
                index = capsLockFlag ? !shiftFlag : shiftFlag;
            }
            buffer[current++] = keyValues[c][index];
			semPost(KEYBOARD_SEM_NAME);
        }
        current %= BUFFER_SIZE; //Limpia ciclicamente el current
    }
}

unsigned char bufferNext() {
	semWait(KEYBOARD_SEM_NAME);
	if (eofFlag) {
	    eofFlag = 0;
	    return EOF;
	}

	if (nextToRead == current) {
		return 0;
	}
	unsigned char toRet = buffer[nextToRead];
	buffer[nextToRead++] = 0;
	nextToRead %= BUFFER_SIZE;

	if (toRet != '6' && toRet != ';' && toRet != ':' && isSpecialKey(toRet)) {
		return bufferNext();
  }

	if(ctrlFlag) {
		switch (toRet) {
			case 's':
			case 'S':
				regShotFlag = 1;
				updateRegisters();
				toRet = '\n';
				break;
			case 'c':
			case 'C':
				sys_write(1, "^C", 2, 0x00FFFFFF);
				killForegroundProcess();
				toRet = '\n';
				break;
			case 'd':
			case 'D':
				sys_write(1, "^D", 2, 0x00FFFFFF);
				sendEOFToSTDIN();
				toRet = '\n';
				break;
		default:
			break;
		}
	}

	if (ctrlFlag && (toRet == 's' || toRet == 'S')) {
		regShotFlag = 1;
		updateRegisters();
		toRet = '\n';
	}
	return toRet;
}

void updateRegisters() {
    _updateRegisters();
    uint64_t * r = _getRegisters();
    for(int i = 0; i < REGS_AMOUNT; i++) {
        registers[i] = r[i];
    }
}

uint64_t getRegisters(uint64_t * r) {
    if(!regShotFlag) {
        return 0;
    }
    for(int i = 0; i < REGS_AMOUNT; i++) {
        r[i] = registers[i];
    }
    return 1;
}

char getRegShotFlag() {
	return regShotFlag;
}

static void sendEOFToSTDIN() {
	eofFlag = 1;
	semPost(KEYBOARD_SEM_NAME);
}
