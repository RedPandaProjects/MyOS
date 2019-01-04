
#pragma aux _kernel_int = \
	"int 0x60"			 \
modify [dx cx bx es]     \  
parm[ax][bx][cx][dx][es] \
value[ax];

int _kernel_int(int ax, int bx, int cx, int dx, int es);

#define REG_INTR(ID,NAME) int INTR_##NAME(char a,int b,int c,int d,int es){return _kernel_int(a|(ID<<8),b,c,d,es);}
#include "INTERS.h"