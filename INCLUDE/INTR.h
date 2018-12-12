#ifndef INTER_H
#define INTER_H
#include "TYPE.h"
struct ByteRegs {
	unsigned :16; unsigned : 16;
	unsigned : 16; unsigned : 16;
	unsigned : 16; unsigned : 16;
	unsigned : 16; unsigned : 16;
	unsigned char	bl, bh;	unsigned char	cl, ch;
	unsigned char	dl, dh;	unsigned char	al, ah;
};
struct WordRegs
{
	unsigned int : 16;
	unsigned int : 16;
	unsigned int es, ds, di, si, bp, sp, bx, dx, cx, ax, ip, cs;
	unsigned int flags;
};

union Regs
{
	struct ByteRegs bReg;
	struct WordRegs wReg;
};
typedef void(INTERRUPT *INTER_FUNC)(union Regs regs);
void    setvect(int inter, INTER_FUNC func);
INTER_FUNC getvect(int inter);
void  __cdecl intr(int inter, union Regs *regs);
#endif