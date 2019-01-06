#include <SYSTEM.h>
#ifndef REG_INTR
#define REG_INTR(ID,NAME) int INTR_##NAME(char a,int b,int c,int d,int es);
#define REG_INTR_FAR(ID,NAME) void FAR* INTR_##NAME(char a,int b,int c,int d,int es);
#endif

REG_INTR(0x0, WRITE)
REG_INTR(0x1, READ)
REG_INTR(0x2, CALL)
REG_INTR_FAR(0x3, MEM)
