
#ifndef REG_INTR
#define REG_INTR(ID,NAME) int INTR_##NAME(char a,int b,int c,int d,int es);
#endif

REG_INTR(0x0, WRITE)
REG_INTR(0x1, READ)