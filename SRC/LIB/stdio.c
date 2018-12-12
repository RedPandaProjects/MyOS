#include <stdio.h>
#pragma aux _putch_ = \
    "mov ah,0x0E"\
	"xor bx,bx"\
	"int 0x10"\
	 modify [bx]\  
	parm  [ax];

void putch(int ch) 
{
	_putch_(ch);
}