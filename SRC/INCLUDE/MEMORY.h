#ifndef MEMORY_H
#define MEMORY_H
#define MEM_BLOCK_FREE 0 
#define MEM_BLOCK 1 
#define MEM_BLOCK_START 2
#include <SYSTEM.h>
extern char MemBlocks[128];
void InitMemory();
int MemoryStatus();
void FAR* MemoryAlloc(unsigned long size);
void MemoryFree(void FAR* ptr);

int Console_MemoryAlloc();
int Console_MemoryFree();
int Console_MemoryStatus();
int Console_MemorySet();
int Console_MemoryCall();
int Console_MemoryTo();
#endif
