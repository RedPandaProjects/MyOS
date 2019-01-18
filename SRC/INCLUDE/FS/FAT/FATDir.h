#ifndef FATDIR_H
#define FATDIR_H
#include "FAT.h"
int   FATGetItem(FATEntries*item);
int   FATNextItem(FATEntries*item);
void  FATSaveItem(FATEntries*item);
int   FATAllocItem(FATEntries*item);
void  FATFreeItem(FATEntries*item);
void	FATOpenDir(FATEntries*item);
void	FATCloseDir();
#endif
