#include "SYSTEM.h"
#include "INTERS.h"
void farfree(void FAR* block)
{
	INTR_MEM(1, TOL((unsigned long)block), 0, 0, TOU((unsigned long)block));
}

void FAR* farmalloc(unsigned long nbytes)
{
	return INTR_MEM(0, TOL(nbytes), TOU(nbytes),0,0);
}

void FAR* farrealloc(void FAR* oldblock, unsigned long nbytes)
{
	return (void FAR*)NULL;
}
