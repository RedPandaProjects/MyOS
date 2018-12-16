#ifndef TYPE_H
#define TYPE_H
#ifdef VS
#define INTERRUPT 
#define RTLINK
#define FAR
#else
#define FAR __far
#define INTERRUPT  __interrupt
#define RTLINK __declspec(__watcall)
#endif
void __cdecl FAR*get_far(int seg,int adr);
 int __cdecl get_ds();
#endif