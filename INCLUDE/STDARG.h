#ifndef STDARG_H
#define STDARG_H

typedef char	*va_list;
#define _va_jmp(a) ((sizeof(a) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
#define va_start(ap,v) (ap = ((va_list)&v)+_va_jmp(v) /* + sizeof(v) + sizeof(v)%sizeof(int)-1*/)
#define va_arg(ap,t) *((t *)((ap+=_va_jmp(t))-_va_jmp(t)/* +=( sizeof(t) + sizeof(t)%sizeof(int)))-(sizeof(t) + sizeof(t)%sizeof(int))*/))
#define va_end(ap) ap = 0

#endif
