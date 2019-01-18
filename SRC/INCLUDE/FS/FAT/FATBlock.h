#ifndef FATBLOCK_H
#define FATBLOCK_H
#include <SYSTEM.h>
extern unsigned int GFSFatSectorID;
typedef struct 
{
	unsigned int(*AllocBlock)();
	unsigned int(*AllocNextBlock)(unsigned int);
	unsigned int(*NextBlock)(unsigned int);
	void(*FreeBlocks)(unsigned int);
	void(*CutNextBlocks)(unsigned int);
} FATBlockFunctions;
extern FATBlockFunctions GFATBlockFunctions;

extern unsigned int GClysterID;
extern void FAR*GFATClyster;
int FATReadClyster(unsigned int id);
int FATWriteClyster(unsigned int id);
#endif
