#include <SYSTEM.h>
#pragma aux _kernel_int = \
	"int 0x60"			 \
modify [dx cx bx es]     \  
parm[ax][bx][cx][dx][es] \
value[ax];

int _kernel_int(int ax, int bx, int cx, int dx, int es);

#pragma aux _kernel_int_far = \
	"int 0x60"			 \
modify [dx cx bx es]     \  
parm[ax][bx][cx][dx][es] \
value[es ax];

int _kernel_int(int ax, int bx, int cx, int dx, int es);
void FAR* _kernel_int_far(int ax, int bx, int cx, int dx, int es);

#define REG_INTR(ID,NAME) int INTR_##NAME(char a,int b,int c,int d,int es){return _kernel_int(a|(ID<<8),b,c,d,es);}
#define REG_INTR_FAR(ID,NAME) void FAR* INTR_##NAME(char a,int b,int c,int d,int es){return _kernel_int_far(a|(ID<<8),b,c,d,es);}
#include "INTERS.h"
