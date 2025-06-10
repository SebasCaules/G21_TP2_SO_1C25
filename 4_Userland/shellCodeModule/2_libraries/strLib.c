// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <strLib.h>

uint64_t strlen(const char *str) {
    const char *s = str;
    while (*s) {
        ++s;
    }
    return s - str;
}

int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

// Copia todo el string src en dst, incluyendo el '\0'
char *strcpy(char *dst, const char *src) {
    char *ret = dst;
    while ((*dst++ = *src++)) {
        ; // Copia hasta que src sea '\0'
    }
    return ret;
}

// Copia hasta n caracteres de src a dst
char *strncpy(char *dst, const char *src, int n) {
    char *ret = dst;
    while (n > 0 && *src) {
        *dst++ = *src++;
        n--;
    }
    while (n > 0) {
        *dst++ = '\0'; // Rellenamos con '\0' si src terminó antes
        n--;
    }
    return ret;
}

char toLowercase(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }
    return c;
}

char toUppercase(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 32;
    }
    return c;
}

char isUpper(char c) {
    if((c >= 33 && c <= 43) || (c >= 65 && c <= 90)) {
        return 1;
    }
    return 0;
}

int isspace(int c) {
    return (c == ' ' || c == '\t' || c == '\n' ||
            c == '\v' || c == '\f' || c == '\r');
}

int splitString(const char* str, char words[MAX_WORDS][MAX_WORD_LENGTH]) {
    int wordCount = 0;
    int charIndex = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (isspace(str[i])) {
            if (charIndex > 0) {
                words[wordCount][charIndex] = '\0';
                wordCount++;
                charIndex = 0;

                if (wordCount >= MAX_WORDS) {
                    break;
                }
            }
        } else {
            if (charIndex < MAX_WORD_LENGTH - 1) {
                words[wordCount][charIndex++] = str[i];
            }
        }
    }

    if (charIndex > 0 && wordCount < MAX_WORDS) {
        words[wordCount][charIndex] = '\0';
        wordCount++;
    }

    return wordCount;
}


// Manejo de Numeros y Strings

uint64_t intToString(int value, char* buffer, uint64_t length) {
    int isNegative = 0;
    uint64_t i = length;
    if (value < 0) {
        isNegative = 1;
        value = -value;  
    }
    do {
        buffer[i++] = (value % 10) + '0';
        value /= 10;
    } while (value > 0);
    if (isNegative) {
        buffer[i++] = '-';
    }

	int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
	return i;
}

uint64_t decimalToString(double value, char* buffer){
	int intPart = value;
	uint64_t length = 0;
	length += intToString(intPart, buffer, length);
	buffer[length++] = '.';

	char auxBuffer[BUFFER_SIZE];
	uint64_t auxLength = 0;
	int decimalPart = (value - intPart) * 10;
	auxLength += intToString(decimalPart, auxBuffer, auxLength);
	for (uint8_t i = 0; i < auxLength; i++) {
		buffer[length++] = auxBuffer[i];
	}
	return length;
}

int convert_to_base_string(int number, int base, char *buffer) {
    const char *digits = "0123456789ABCDEF";
    char temp[BUFFER_SIZE];
    int pos = 0, len = 0;
    
    if (base < 2 || base > 16) {
        return -1;
    }

    if (base == 16) {
        buffer[len++] = '0';
        buffer[len++] = 'x';
    } else if (base == 2) {
        buffer[len++] = '0';
        buffer[len++] = 'b';
    }

    if (number == 0) {
        buffer[len++] = '0';
        buffer[len] = '\0';
        return len;
    }

    if (number < 0 && base == 10) {
        temp[pos++] = '-';
        number = -number;
    }

    while (number) {
        temp[pos++] = digits[number % base];
        number /= base;
    }

    while (pos > 0) {
        buffer[len++] = temp[--pos];
    }
    buffer[len] = '\0';

    return len;
}

int stringToInt(const char *str) {
    int result = 0;
    int i = 0;

    while (str[i] == ' ') {
        i++;
    }

    if (str[i] == '\0') {
        return -1;
    }

    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    if (str[i] != '\0') {
        return -1;
    }

    return result;
}

int isNumber(char c) {
    return (c >= '0' && c <= '9');
}

int isCharacter(char c) {
    return ((c >= 'a' && c <= 'z'));
}

char *strchr(const char *str, int c) {
	for (int i = 0; str[i] != 0; i++) {
		if (str[i] == c) {
			return (char *) &str[i];
		}
	}
	return NULL;
}
