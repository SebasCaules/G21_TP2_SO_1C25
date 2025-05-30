#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

int strlength(const char *str);

void strncopy(char *dest, const char *src, uint64_t size);

char * itoaHex(uint64_t num, char * str);

char *itoa(int num, char *str);

int strncat(char *dest, const char *src);

int strcmp(const char *s1, const char *s2);

#endif