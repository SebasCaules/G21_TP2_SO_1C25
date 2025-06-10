// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <naiveConsole.h>

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25;

void ncPrint(const char * string)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

// uint64_t printStrByLength(char* str, int fgcolor, int bgcolor, int length){
//     uint64_t i, printed = 0;
// 	for (i = 0; i < length && str[i] != 0; i++)
// 		printed += printChar(str[i], fgcolor, bgcolor);
// 	return printed;
// }

uint64_t ncPrintStrByLength(char* str, int fgcolor, int bgcolor, int length){
	uint64_t i, printed = 0;
	for (i = 0; i < length && str[i] != 0; i++)
		printed += ncPrintCharNew(str[i], fgcolor, bgcolor);
	return printed;
}

uint64_t ncPrintCharNew(char c, int fgcolor, int bgcolor){
	uint64_t printed = 0;
	switch (c) {
		case '\n':
			ncNewline();
			return 1;
		case '\b':
			ncClear();
			return -1;
		case '\t':
			return 0;
		default:
			break;
		}
	
		// Si es un caracter ASCII no imprimible devuelve 0
		if (c <= 31){
			return 0;
		}
	*currentVideo = c;
	currentVideo += 2;
	printed++;
	return printed;
}

void ncPrintChar(char character)
{
	*currentVideo = character;
	currentVideo += 2;
}

void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncClear()
{
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}
