/*
 * OpenOS - String Utility Functions
 * 
 * Basic string manipulation functions for kernel use.
 */

#ifndef OPENOS_KERNEL_STRING_H
#define OPENOS_KERNEL_STRING_H

#include <stddef.h>

/* Get the length of a string */
size_t string_length(const char* str);

/* Compare two strings (returns 0 if equal, non-zero otherwise) */
int string_compare(const char* str1, const char* str2);

/* Copy a string from source to destination */
char* string_copy(char* dest, const char* src);

/* Concatenate two strings */
char* string_concat(char* dest, const char* src);

/* Tokenize a string (similar to strtok) */
char* string_tokenize(char* str, const char* delim);

/* Check if a character is a whitespace character */
int is_whitespace(char c);

/* Standard string functions */
size_t strlen(const char* str);
int strcmp(const char* str1, const char* str2);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* ptr, int value, size_t num);
char* strchr(const char* str, int ch);
int strncmp(const char* str1, const char* str2, size_t n);

/* Integer to ASCII conversion */
char* itoa(int value, char* str, int base);

#endif /* OPENOS_KERNEL_STRING_H */
