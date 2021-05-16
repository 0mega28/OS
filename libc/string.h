#ifndef STRING_H
#define STRING_H

#include <stdint.h>

void int_to_ascii(int n, char str[]);
void reverse(char s[]);
int strlen(char s[]);
void backspace(char s[]);
void append(char s[], char n);
int strcmp(char s1[], char s2[]);
void hex_to_ascii(int n, char str[]);

/* Copy from src to dest */
void string_copy(char *dest, const char *src);

/* Concatenate string src in dest */
void string_cat(char *dest, const char *src);

/* Returns token of string separated by delimiter d at index i */
char* strtok(char *string, char d, int i);

#endif