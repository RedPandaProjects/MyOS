#include <ABORT.h>
#undef abort
#include <STDIO.h>
#include "DISPLAY.h"
void abort(const char*name, unsigned int  id, unsigned int  a, unsigned int  b, unsigned int  c, unsigned int  d)
{
	ClearScreen();
	printf("ABORT!!!\r\n");
	printf("%s:%x\r\n", name, id);
	printf("[%x][%x][%x][%x]\r\n", a,b,c,d);
	while (1) {}
}
