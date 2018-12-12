
#ifndef REG_INTR
#define REG_INTR(ID,NAME) int INTR_##NAME(int a,int b,int c,int d)
#endif

REG_INTR(0x0, WRITE);
REG_INTR(0x1, READ);