#ifndef STRING_H
#define STRING_H
#include <SYSTEM.h>
char FAR* strcpy(char FAR* dest, const char FAR*src);

char FAR* strncpy(char FAR* dest, const char FAR*src, int count);
char FAR* strcat(char FAR* dest, const char FAR* src);

char FAR* strncat(char FAR* dest, const char FAR* src, int count);

int strcmp(const char FAR* cs, const char FAR* ct);
int strncmp(const char FAR* cs, const char FAR* ct, int count);

char FAR* strchr(const char FAR* s, int c);

char FAR* strrchr(const char FAR* s, int c);

int    strspn(const char FAR* cs, const char FAR* ct);

int    strcspn(const char FAR* cs, const char FAR* ct);
char FAR* strpbrk(const char FAR* cs, const char FAR* ct);

char FAR* strstr(const char FAR* cs, const char FAR* ct);

int strlen(const char FAR* s);
char FAR* strtok(char FAR* s, const char FAR* ct);

void FAR* memcpy(void FAR* dest, const void FAR* src, int n);
void FAR* memmove(void FAR* dest, const void FAR* src, int n);
int    memcmp(const void FAR* cs, const void FAR* ct, int count);
void FAR* memchr(const void FAR* cs, int c, int count);
void FAR* memrchr(const void FAR* cs, int c, int count);
void FAR* memset(void FAR* s, int c, int count);
#endif
