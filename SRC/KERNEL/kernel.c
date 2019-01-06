#include <INTR.h>
#include <DISPLAY.h>
#include <CONSOLE.h>
#define  REG_INTR(ID,NAME) INTR_##NAME=ID,
#define  REG_INTR_FAR(ID,NAME) INTR_##NAME=ID,
enum Intrs
{
#include <INTERS.h>
};
 

void Kernel(union Regs FAR*regs)
{
	switch (regs->bReg.ah)
	{
	case INTR_WRITE:
		if (regs->wReg.bx == -1)
		{
			PrintText(get_far(regs->wReg.es, regs->wReg.dx), regs->wReg.cx);
		}
		break;
	case INTR_READ:
		break;
	case INTR_CALL:
		regs->wReg.ax = ConsoleCall(get_far(regs->wReg.es, regs->wReg.bx));
		break;
	default:
		break;
	}
	
}

void INTERRUPT KernelInterrupt(union Regs regs)
{
	Kernel(&regs);
}
void InitKernel()
{
	setvect(0x60, KernelInterrupt);
}
