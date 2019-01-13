#ifndef DISK_H
#define DISK_H
#include <SYSTEM.h>
extern int GCurrentDisk;
void InitDisk(int id);
struct DiskInfo
{
	int SizeSector;
	int CntSector;
	int CntCylinder;
	int CntHead;
};
extern struct DiskInfo GDiskInfo;
int ReadSector(unsigned long x,void FAR*out);
int WriteSector(unsigned long x,const void FAR*out);
#define DISK_DEBUG
#ifdef DISK_DEBUG
int Console_DiskRead();
#endif
#endif
