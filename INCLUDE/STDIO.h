#ifndef STDIO_H
#define STDIO_H
#include <STDARG.h>

#define stdout ((FILE *)-1)
typedef int FILE;

int    __cdecl printf(const char *format,...);
int    __cdecl   vfprintf(FILE *stream, const char *format, va_list arglist);
int   __cdecl    vfscanf(FILE *stream, const char *format, va_list arglist);
int   __cdecl    vprintf(const char *format, va_list arglist);
int   __cdecl    vscanf(const char *format, va_list arglist);
int   __cdecl    vsprintf(char *buffer, const char *format, va_list arglist);
int    __cdecl   vsscanf(const char *buffer, const char *format, va_list arglist);

void    __cdecl  putch(int);
 
#endif
