#include <stdio.h>
#include <intr.h>
#include <BIOS.h>

#pragma aux _putch_ = \
    "mov ah,0x0E"\
	"xor bx,bx"\
	"int 0x10"\
	 modify [bx]\  
parm[ax];
int KernelMain(int drive)
{
	InitKernel();
	printf("test %s %c %s %lld \r\n","bla bla barada",(char)0x1,"test",0xffffffffffffffff);
	while(1){}
	return 0;
}
