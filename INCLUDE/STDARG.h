#ifndef STDARG_H
#define STDARG_H

typedef char	*va_list;

#define va_start(ap,v) ap = (va_list)&v + sizeof(v)
#define va_arg(ap,t) ((t *)(ap += sizeof(t)))[-1]
#define va_end(ap) ap = NULL

#endif