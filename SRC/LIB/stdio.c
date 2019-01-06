#include <stdio.h>
#include <CTYPE.h>
#include "INTERS.h"

void __cdecl putch(int ch)
{
	INTR_WRITE(0, (int)stdout, 1, (int)&ch, get_ds());
}
 
int print_string(FILE *stream, const char*str)
{
	while (str&&*str)
	{
		INTR_WRITE(0, (int)stream, 1, (int)str, get_ds());
		str++;
	}
	return 0;
}
int print_string_from_va_list(FILE *stream, va_list *arglist)
{
	const char* str = va_arg(*arglist, const char*);
	if (str == 0)return 0;
	print_string(stream, str); 
	return 0; 
}

#define N_16 0x2
#define N_SIG 0x1
#define N_MINUS 0x1 
void print_number(int bytes,int flags,FILE *stream, va_list *arglist)
{
	char out[22];
	char * begin = (char*)out;
	unsigned long long number = 0;
	out[21] = 0;
	begin +=20;
	switch (bytes)
	{
	case sizeof(char):
		number = va_arg(*arglist,unsigned char);
		if ((flags&N_SIG) && (number & 1 << 7))
		{
			number = -(char)number;
			flags |= N_MINUS;
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
				number = -(int)number;
				flags |= N_MINUS;
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
				number = -(int)number;
				flags |= N_MINUS;
			}
			else
			{
				flags &= ~N_MINUS;
			}
			break;
#endif
	case sizeof(long):
		number = va_arg(*arglist, unsigned long);
		if ((flags&N_SIG) && (number & 0x80000000))
		{
			number = -(long)number;
			flags |= N_MINUS;
		}
		else
		{
			flags &= ~N_MINUS;
		}

		break;
	case sizeof(long long) :
		number = va_arg(*arglist, unsigned long long);
		if ((flags&N_SIG) && (number & 0x8000000000000000))
		{
			number = -number;
			flags |= N_MINUS;
		}
		else
		{
			flags &= ~N_MINUS;
		}
		break;
	default:
		return;
		break;
	}
	if (number == 0)
	{
		print_string(stream, "0");
		return;
	}
	if (flags&N_16)
	{
		while (number)
		{
			*begin = number % 0x10;
			number = number / 0x10;
			if (*begin > 9)
				*begin += 'A'-10;
			else
				*begin += '0';
			if (number)
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
			if(number)
				begin--; 
		}
		if (flags&N_MINUS)
		{
			print_string(stream, "-");
		}
	} 
	print_string(stream, begin);
}
int __cdecl printf(const char * format, ...)
{ 
	va_list va;
	va_start(va, format);
	vprintf(format, va);
	va_end(va); 
	return 0;
} 
int    __cdecl   vfprintf(FILE *stream, const char *format, va_list arglist)
{
	char ch = 0;
	while (*format)
	{
		if (*format == '%')
		{
			int stp = 1;
			format++;

			if (*format == '%')
			{
				ch = '%';
			} 
			else if (*format == 'c')
			{
				ch = va_arg(arglist, char);
				INTR_WRITE(0, (int)stream, 1, (int)&ch, get_ds());
				format++;
				continue;
			}
			else if (*format == 's')
			{
				print_string_from_va_list(stream,&arglist);
				format++;
				continue;
			}
			else
			{
				if (*format == 'l' || *format == 'h' || *format == 'd' || *format == 'u' || *format == 'x')
				{
					while (*format)
					{
						if (*format == 'l')
						{
							stp++;
						}
						else if (*format == 'h')
						{
							stp--;
						}
						else
						{
							break;
						}
						format++;
					}
					if (stp < 0)stp = 0;
					else if (stp > 3)stp = 3;

					stp = 1 << stp;
					
					if (*format == 'u')
					{
						print_number(stp, 0, stream, &arglist);
					}
					else if (*format == 'd')
					{
						print_number(stp, N_SIG, stream, &arglist);
					}
					else if (*format == 'x')
					{
						print_number(stp, N_16, stream, &arglist);
					}
					format++;
					continue;
				}

			}
			format++;
		}
		else
		{
			ch = *format;
		}
		INTR_WRITE(0, (int)stream, 1, (int)&ch, get_ds());
		format++;
	}
	return 0; 
}
int     __cdecl  vprintf(const char *format, va_list arglist)
{
	return vfprintf(stdout, format,arglist);
}
int   __cdecl    vscanf(const char *format, va_list arglist)
{
	return 0;
}
