#include <stdio.h>
#include "INTERS.h"

void putch(int ch) 
{
	_putch_(ch);
}

int print_number(FILE *stream,)
{

}

int    __cdecl   vfprintf(FILE *stream, const char *format, va_list arglist)
{
	char ch = 0;
	while (*format)
	{
		if (format == '%')
		{
			int stp = 0;
			format++;
			switch (*format)
			{
			case '%':
				ch = '%';
				break;
			case 'u':
				ch = 'u';
			case 'l':
				stp++;
				break;
			case 'h':
				stp--;
				break;
			default:
				continue;
				break;
			}
		}
		else
		{
			ch = *stream;
		}
		INTR_WRITE(0, (int)stream, 1, &ch, get_ds());
		format++;
	}
}
int     __cdecl  vprintf(const char *format, va_list arglist)
{
	vprintf(stdout, arglist);
}
int   __cdecl    vscanf(const char *format, va_list arglist)
{

}