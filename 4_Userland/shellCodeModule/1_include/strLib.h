#include <stdint.h>

#ifndef TPE_ARQUI_STRLIB_H
#define TPE_ARQUI_STRLIB_H

#define MAX_WORDS 4
#define MAX_WORD_LENGTH 32
#define BUFFER_SIZE 1024

/**
 * @brief Calculates the length of a string.
 * @param str The string to measure.
 * @return The length of the string.
 */
uint64_t strlen(const char *str);

/**
 * @brief Compares two strings lexicographically.
 * @param str1 The first string to compare.
 * @param str2 The second string to compare.
 * @return An integer less than, equal to, or greater than zero if str1 is found, respectively, to be less than, to match, or to be greater than str2.
 */
int strcmp(const char *str1, const char *str2);

/**
 * @brief checks if a character is uppercase.
 * @param c The character to check.
 * @return Returns 1 if c is uppercase, 0 if not.
 */
char isUpper(char c);

/**
 * @brief Converts a character to lowercase.
 * @param c The character to convert.
 * @return The lowercase equivalent of the character, or the character itself if it's not uppercase.
 */
char toLowercase(char c);

/**
 * @brief Converts a character to uppercase.
 * @param c The character to convert.
 * @return The uppercase equivalent of the character, or the character itself if it's not lowercase.
 */
char toUppercase(char c);

/**
 * @brief Checks if a character is a whitespace character.
 * @param c The character to check.
 * @return Non-zero if the character is a whitespace character, zero otherwise.
 */
int isspace(int c);

/**
 * @brief Splits a string into words, storing them in an array.
 * @param str The string to split.
 * @param words The array to store the words, with a maximum size of MAX_WORDS and each word having a maximum length of MAX_WORD_LENGTH.
 * @return The number of words found in the string.
 */
int splitString(const char* str, char words[MAX_WORDS][MAX_WORD_LENGTH]);

/**
 * @brief Converts an integer to a string.
 * @param value The integer to convert.
 * @param buffer The buffer to store the resulting string.
 * @param length The maximum length of the buffer.
 * @return The length of the resulting string.
 */
uint64_t intToString(int value, char* buffer, uint64_t length);

/**
 * @brief Converts a decimal (double) value to a string representation.
 * @param value The double value to convert.
 * @param buffer The buffer to store the resulting string.
 * @return The length of the resulting string.
 */
uint64_t decimalToString(double value, char* buffer);

/**
 * @brief Converts an integer to a string in the specified base.
 * @param number The integer to convert.
 * @param base The base to convert to (e.g., 2 for binary, 10 for decimal).
 * @param buffer The buffer to store the resulting string.
 * @return The length of the resulting string.
 */
int convert_to_base_string(int number, int base, char *buffer);

/**
 * @brief Converts a string representation of an integer to an integer value.
 * @param str The string to convert.
 * @return The integer value of the string.
 */
int stringToInt(const char *str);

/**
 * @brief Checks if a character is a numerical digit.
 * @param c The character to be checked.
 * @return 1 if the character is a digit (0-9), 0 otherwise.
 */
int isNumber(char c);

/**
 * @brief Checks if a character is an alphabetical letter.
 * @param c The character to be checked.
 * @return 1 if the character is a letter (a-z, A-Z), 0 otherwise.
 */
int isCharacter(char c);

#endif //TPE_ARQUI_STRLIB_H

