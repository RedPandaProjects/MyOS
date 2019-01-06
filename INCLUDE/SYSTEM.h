#ifndef SYSTEM_H
#define SYSTEM_H

#ifdef VS
#define INTERRUPT 
#define RTLINK
#define FAR
#else
#define FAR __far
#define INTERRUPT  __interrupt
#define RTLINK __declspec(__watcall)
#endif

#define NULL 0

#define TOL(a) (unsigned int)(a&0xFFFF)
#define TOU(a) (unsigned int)(a>>8)

void  FAR* __cdecl get_far(int seg, int adr);
int  __cdecl get_ds();

void       farfree(void FAR *block);
void FAR  * farmalloc(unsigned long nbytes);
void FAR  * farrealloc(void FAR *oldblock, unsigned long nbytes);
#endif 
