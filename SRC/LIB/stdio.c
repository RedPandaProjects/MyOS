#include <stdio.h>
#include "INTERS.h"

void putch(int ch) 
{
	_putch_(ch);
}

#define N_16 0x2
#define N_SIG 0x1
#define N_MINUS 0x1
int print_number(int bytes,int flags,FILE *stream, va_list *arglist)
{
	char out[22];out[21] = 0;
	char *begin = out + 20;
	unsigned long long number = 0;
	switch (bytes)
	{
	case sizeof(char):
		number = va_arg(*arglist,unsigned char);
		if ((flags&N_SIG) && (number & 1 << 7))
		{
			number = number & (~(1 << 7));
			number = 0x7F & (~number);
		}
		else
		{
			flags &= ~N_MINUS;
		}
		break;
#if _MSC_VER
		case sizeof(short) :
			number = va_arg(*arglist, unsigned int);
			if ((flags&N_SIG) && (number & 1 << 15))
			{
				number = number & (~(1 << 15));
				number = 0x7FFF & (~number);
			}
			else
			{
				flags &= ~N_MINUS;
			}
			break;
#else 
		case sizeof(int) :
			number = va_arg(*arglist, unsigned int);
			if ((flags&N_SIG) && (number & 1 << 15))
			{
				number = number & (~(1 << 15));
				number = 0x7FFF & (~number);
			}
			else
			{
				is_signed = 0;
			}
			break;
#endif
	case sizeof(long):
		number = va_arg(*arglist, unsigned long);
		if ((flags&N_SIG) && (number & 1 << 31))
		{
			number = number & (~(1 << 31));
			number = 0x7FFFFFFF & (~number);
		}
		else
		{
			flags &= ~N_MINUS;
		}

		break;
	case sizeof(long long) :
		number = va_arg(*arglist, long long);
		number = va_arg(*arglist, unsigned long);
		if ((flags&N_SIG) && (number & 1 << 63))
		{
			number = number & (~(1 << 31));
			number = 0x7FFFFFFFFFFFFFFF & (~number);
		}
		else
		{
			flags &= ~N_MINUS;
		}
		break;
	default:
		va_arg(*arglist, int);
		return;
		break;
	}

	if (flags&N_16)
	{
		while (number)
		{
			*begin = number % 0x10;
			number = number / 0x10;
			if (*begin >= 9)
				*begin += 'A'-9;
			else
				*begin += '0';
		
			begin--;
		}
	}
	else
	{
		while (number)
		{
			*begin = number % 10;
			number = number / 10;
			*begin += '0';
			begin--;
		}
	}

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
			case 'l':
				stp++;
				break;
			case 'h':
				stp--;
				break;
			default:
				break;
			}
			switch (*format)
			{
				if()
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