#include <intr.h>


struct INTERRUPT_VECTOR
{
	INTER_FUNC function;
};
static struct INTERRUPT_VECTOR FAR*vector = (struct INTERRUPT_VECTOR FAR*) 0x0;

#pragma aux             _DoINTR parm [bx] [ax dx] modify [cx si di es];

void    setvect(int inter, INTER_FUNC func)
{
	vector[inter].function = func;
	
}
INTER_FUNC getvect(int inter)
{
	return  vector[inter].function;
}

