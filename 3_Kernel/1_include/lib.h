#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

int strlength(const char *str);

void strncopy(char *dest, const char *src, uint64_t size);

char * itoaHex(uint64_t num, char * str);

#endif