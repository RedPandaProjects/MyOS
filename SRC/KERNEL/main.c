#include <stdio.h>
#include <intr.h>
#pragma aux init = \
	"mov ax,0x0" \
	"int 0x21" \
  modify [ax] ;   
extern void init();    
#pragma aux init2 = \
	"mov ax,0x0e01" \
	"int 0x10" \
  modify [ax] ;   
extern void init();
char* text="hello world watcom c++ 8086";
void INTERRUPT test_int(union Regs rg);
int KernelMain(int drive)
{
		setvect(0x21, test_int);
	//	union Regs *tr;
		//intr(0x21, tr);
		init();
	while(1){}
	return 0;
}
void INTERRUPT test_int(union Regs rg)
{
	union Regs rgs;
	rgs.bReg.al = 0x1;
	rgs.bReg.ah = 0x0e;
	rgs.wReg.bx = 0;
	intr(0x10, &rgs);
	rgs.bReg.al ='a';
	rgs.bReg.ah = 0x0e;
	rgs.wReg.bx = 0;
	intr(0x10, &rgs);
}