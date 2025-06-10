// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
// Keyboard Dirver de arqui deprecado para implementarlo con semáforos y circular buffer

// #include <stdint.h>
// #include "videoDriver.h"
// #include "keyboard.h"
// #include "synchro.h"
// #include "scheduler.h"

// #define NKEYS 58
// #define BUFFER_SIZE 256
// #define MAX_PRESS_CODE 0x70
// #define REGS_AMOUNT 17

// // Define de teclas especiales:
// #define ESC 0x01
// #define ENTER 0x1C
// #define BACKSPACE 0x0E
// #define TAB 0x0F
// #define L_SHIFT_PRESS 0x2A
// #define L_SHIFT_RELEASE 0xAA
// #define R_SHIFT_PRESS 0x36
// #define R_SHIFT_RELEASE 0xB6
// #define CAPS_LOCK_PRESS 0x3A
// #define ALT_PRESS 0x3B
// #define ALT_RELEASE 0xB8

// #define UP_ARROW 0x48
// #define DOWN_ARROW 0x50
// #define LEFT_ARROW 0x4B
// #define RIGHT_ARROW 0x4D

// #define CTRL_PRESS 0x1D
// #define CTRL_RELEASE 0x9D

// #define KEYBOARD_SEM_NAME "keyboard_sem"
// #define EOF -1

// extern void _updateRegisters();
// extern uint64_t * _getRegisters();
// static void updateRegisters();
// static void sendEOFToSTDIN();

// static uint8_t regShotFlag = 0;
// static char eofFlag = 0;

// static volatile uint64_t registers[REGS_AMOUNT];

// static unsigned char keyValues[NKEYS][2] = {
// 	{0, 0}, {27, 27}, {'1', '!'}, {'2', '@'}, {'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '^'}, {'7', '&'},
// 	{'8', '*'}, {'9', '('}, {'0', ')'}, {'-', '_'}, {'=', '+'}, {'\b', '\b'}, {9, 9}, {'q', 'Q'}, {'w', 'W'}, 
// 	{'e', 'E'}, {'r', 'R'}, {'t', 'T'}, {'y', 'Y'}, {'u', 'U'}, {'i', 'I'}, {'o', 'O'}, {'p', 'P'}, {'[', '{'},
// 	{']', '}'}, {'\n', '\n'}, {0, 0}, {'a', 'A'}, {'s', 'S'}, {'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'},
// 	{'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {';', ':'}, {39, 34}, {'`', '~'}, {0, 0}, {'\\', '|'}, {'z', 'Z'},
// 	{'x', 'X'}, {'c', 'C'}, {'v', 'V'}, {'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',', '<'}, {'.', '>'}, {'/', '?'},
// 	{0, 0}, {0, 0}, {0, 0}, {' ', ' '},
// };

// static char isSpecialKey(int key) {
//     switch(key) {
//         case L_SHIFT_PRESS:
//         case R_SHIFT_PRESS:
//         case CAPS_LOCK_PRESS:
//         case ALT_PRESS:
//         case ESC:
//         case UP_ARROW:
//         case DOWN_ARROW:
//         case LEFT_ARROW:
//         case RIGHT_ARROW:
//             return 1;
//         default:
//             return 0;
//     }
// }

// int initKeyboardDriver() {
// 	if (semOpen(KEYBOARD_SEM_NAME, 0) == -1) {
// 		return -1;
// 	}
// 	return 0;
// }

// static char buffer[BUFFER_SIZE];
// static int current = 0;
// static int nextToRead = 0;

// int shiftFlag = 0;
// int capsLockFlag = 0;
// int ctrlFlag = 0;

// void pressedKey(){
// 	int c = getPressedKey();

// 	switch (c) {
// 	case L_SHIFT_PRESS:
// 	case R_SHIFT_PRESS:
// 			shiftFlag = 1;
// 			break;
// 	case L_SHIFT_RELEASE:
// 	case R_SHIFT_RELEASE:
// 			shiftFlag = 0;
// 			return;
// 	case CAPS_LOCK_PRESS:
// 			capsLockFlag = !capsLockFlag;
// 			break;
// 	case CTRL_PRESS:
// 			ctrlFlag = 1;
// 			break;
// 	case CTRL_RELEASE:
// 			ctrlFlag = 0;
// 			break;
// 	}

// 	if (c <= MAX_PRESS_CODE) {
// 		if(isSpecialKey(c)) {
// 				buffer[current++] = c;
// 		} else {
// 				int index = shiftFlag;
// 				//Chequea que la tecla apretada sea una letra, si lo es y el capslock esta activado se van a poner en mayus
// 				if(keyValues[c][0] >= 'a' && keyValues[c][0] <= 'z'){
// 						index = capsLockFlag ? !shiftFlag : shiftFlag;
// 				}
// 				buffer[current++] = keyValues[c][index];
// 			}
// 		current %= BUFFER_SIZE; //Limpia ciclicamente el current
// 		semPost(KEYBOARD_SEM_NAME);
// 	}
// }

// unsigned char bufferNext() {
// 	if (eofFlag) {
// 		eofFlag = 0;
// 		return EOF;
// 	}

// 	if (nextToRead == current) {
// 		return 0;
// 	}
// 	unsigned char toRet = buffer[nextToRead];
// 	buffer[nextToRead++] = 0;
// 	nextToRead %= BUFFER_SIZE;

// 	if (toRet != '6' && toRet != ';' && toRet != ':' && isSpecialKey(toRet)) {
// 		return bufferNext();
//   }

// 	if(ctrlFlag) {
// 		switch (toRet) {
// 			case 's':
// 			case 'S':
// 				regShotFlag = 1;
// 				updateRegisters();
// 				toRet = '\n';
// 				break;
// 			case 'c':
// 			case 'C':
// 				sys_write(1, "^C", 2, 0x00FFFFFF);
// 				killForegroundProcess();
// 				toRet = '\n';
// 				break;
// 			case 'd':
// 			case 'D':
// 				sys_write(1, "^D", 2, 0x00FFFFFF);
// 				sendEOFToSTDIN();
// 				toRet = '\n';
// 				break;
// 		default:
// 			break;
// 		}
// 	}

// 	if (ctrlFlag && (toRet == 's' || toRet == 'S')) {
// 		regShotFlag = 1;
// 		updateRegisters();
// 		toRet = '\n';
// 	}
// 	return toRet;
// }

// char kb_getchar() {
// 	updateStdinWait(1);
// 	semWait(KEYBOARD_SEM_NAME);
// 	updateStdinWait(0);
// 	return bufferNext();
// }

// void updateRegisters() {
//     _updateRegisters();
//     uint64_t * r = _getRegisters();
//     for(int i = 0; i < REGS_AMOUNT; i++) {
//         registers[i] = r[i];
//     }
// }

// uint64_t getRegisters(uint64_t * r) {
//     if(!regShotFlag) {
//         return 0;
//     }
//     for(int i = 0; i < REGS_AMOUNT; i++) {
//         r[i] = registers[i];
//     }
//     return 1;
// }

// char getRegShotFlag() {
// 	return regShotFlag;
// }

// static void sendEOFToSTDIN() {
// 	eofFlag = 1;
// 	semPost(KEYBOARD_SEM_NAME);
// }

// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboard.h>
#include <scheduler.h>
#include <semaphore.h>
#include <syscalls.h>

#define KB_SEM_NAME "os_kb_sem"
#define EOF -1

extern void _updateRegisters();
extern uint64_t * _getRegisters();
static char scancodeToAscii(uint8_t scancode);
static void updateFlags(uint8_t scancode);
static int cb_isEmpty();
static char cb_push(char c);
static char cb_pop();
static int cb_isfull();
static void updateRegisters();
static void send_eof_to_stdin();
#define KEYS_AMOUNT 59
#define ESC 27

/*
 * Keycode matrix:
 * in index zero the char without shift, in index 1 the char with shift
 * 0 if the key doesn't have a char representation
 */
static char keycodeMatrix[KEYS_AMOUNT][2] = {
	{0, 0}, {ESC, ESC}, {'1', '!'}, {'2', '@'}, {'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '^'}, {'7', '&'}, {'8', '*'}, {'9', '('}, {'0', ')'}, {'-', '_'}, {'=', '+'}, {'\b', '\b'}, {'\t', '\t'}, {'q', 'Q'}, {'w', 'W'}, {'e', 'E'}, {'r', 'R'}, {'t', 'T'}, {'y', 'Y'}, {'u', 'U'}, {'i', 'I'}, {'o', 'O'}, {'p', 'P'}, {'[', '{'}, {']', '}'}, {'\n', '\n'}, {0, 0}, {'a', 'A'}, {'s', 'S'}, {'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'}, {'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {';', ':'}, {'\'', '\"'}, {'`', '~'}, {0, 0}, {'\\', '|'}, {'z', 'Z'}, {'x', 'X'}, {'c', 'C'}, {'v', 'V'}, {'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',', '<'}, {'.', '>'}, {'/', '?'}, {0, 0}, {0, 0}, {0, 0}, {' ', ' '}};

// Circular buffer for keyboard input
#define BUFFER_SIZE 256

typedef struct CircularBuffer {
	char v[BUFFER_SIZE];
	int readIndex;
	int writeIndex;
	int size;
} TCircularBuffer;

static TCircularBuffer buffer = {.readIndex = 0, .writeIndex = 0, .size = 0};

// Special keys flags
static volatile uint8_t activeShift = 0;
static volatile uint8_t activeCapsLock = 0;
static volatile uint8_t activeCtrl = 0;
static volatile uint8_t end_of_file = 0;

static volatile uint64_t registers[17];

static volatile uint8_t registersFilled = 0;

// Static control message arrays for sys_write
static uint16_t ctrlS_msg[] = {'^', 'S'};
static uint16_t ctrlD_msg[] = {'^', 'D'};
static uint16_t ctrlC_msg[] = {'^', 'C'};

int initKeyboardDriver() {
	if (semOpen(KB_SEM_NAME, 0) == -1) {
		return -1;
	}
	return 0;
}

void pressedKey() {
	uint8_t scancode = getPressedKey();
	updateFlags(scancode);
	char ascii = scancodeToAscii(scancode);
	if (activeCtrl) {
		if (ascii == 's' || ascii == 'S') {
			registersFilled = 1;
			updateRegisters();
			sys_write(1, ctrlS_msg, 2);
		}
		else if (ascii == 'd' || ascii == 'D') {
			sys_write(1, ctrlD_msg, 2);
			send_eof_to_stdin();
		}
		else if (ascii == 'c' || ascii == 'C') {
			sys_write(1, ctrlC_msg, 2);
			killForegroundProcess();
		}
	}
	else if (ascii != 0) {
		cb_push(ascii);
		semPost(KB_SEM_NAME);
	}
}

char kb_getchar() {
	updateStdinWait(1);
	semWait(KB_SEM_NAME);
	updateStdinWait(0);
	return cb_pop();
}

static char scancodeToAscii(uint8_t scancode) {
	char ascii = 0;
	if (scancode < KEYS_AMOUNT) {
		ascii = keycodeMatrix[scancode][activeShift];
		if (activeCapsLock && ascii >= 'a' && ascii <= 'z') {
			ascii -= 32;
		}
	}
	return ascii;
}

#define ENTER 0x1C
#define LSHIFT 0x2A
#define RSHIFT 0x36
#define LCTRL 0x1D
#define LCTRL_RELEASE 0x9D
#define CAPSLOCK 0x3A
#define RELEASE_OFFSET 0x80
static void updateFlags(uint8_t scancode) {
	if (scancode == LCTRL) {
		activeCtrl = 1;
	}
	else if (scancode == LCTRL_RELEASE) {
		activeCtrl = 0;
	}
	else if (scancode == LSHIFT || scancode == RSHIFT) {
		activeShift = 1;
	}
	else if (scancode == (LSHIFT + RELEASE_OFFSET) ||
			 scancode == (RSHIFT + RELEASE_OFFSET)) {
		activeShift = 0;
	}
	else if (scancode == CAPSLOCK) {
		activeCapsLock = !activeCapsLock;
	}
}

static int cb_isEmpty() {
	return buffer.readIndex == buffer.writeIndex;
}

static char cb_push(char c) {
	if (cb_isfull()) {
		return 0;
	}
	buffer.v[buffer.writeIndex] = c;
	buffer.writeIndex = (buffer.writeIndex + 1) % BUFFER_SIZE;
	buffer.size++;
	return 1;
}

static int cb_isfull() {
	return buffer.size == BUFFER_SIZE;
}

static char cb_pop() {
	if (cb_isEmpty() && !end_of_file) {
		return 0;
	}

	if (end_of_file) {
		end_of_file = 0;
		return EOF;
	}

	char c = buffer.v[buffer.readIndex];
	buffer.readIndex = (buffer.readIndex + 1) % BUFFER_SIZE;
	buffer.size--;
	return c;
}

void updateRegisters() {
    _updateRegisters();
    uint64_t * r = _getRegisters();
    for(int i = 0; i < 17; i++) {
        registers[i] = r[i];
    }
}

uint64_t getRegisters(uint64_t * r) {
    for(int i = 0; i < 17; i++) {
        r[i] = registers[i];
    }
    return 1;
}

static void send_eof_to_stdin() {
	end_of_file = 1;
	semPost(KB_SEM_NAME);
}

void release_stdin() {
	semPost(KB_SEM_NAME);
}

