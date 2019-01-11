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
#endif
