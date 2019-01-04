#include <INTR.h>
#define  REG_INTR(ID,NAME) INTR_##NAME=ID,
enum Intrs
{
#include <INTERS.h>
};

#pragma aux _putch_ = \
    "mov ah,0x0E"\
	"xor bx,bx"\
	"int 0x10"\
	 modify [bx]\  
parm[ax];

void write_screen(int size,int es,int adrs)
{
	char FAR* text = get_far(es, adrs);
	while (size--)
	{
		_putch_(*text);
		text++;
	}
}

void INTERRUPT KernelInterrupt(union Regs regs)
{
	//_putch_(0x1);
	switch (regs.bReg.ah)
	{
	case INTR_WRITE:
		if (regs.wReg.bx == -1)
		{
			write_screen(regs.wReg.cx, regs.wReg.es, regs.wReg.dx);
		}
		break;
	case INTR_READ:
		break;
	default:
		break;
	}
}
void InitKernel()
{
	setvect(0x60, KernelInterrupt);
}