#include <DISK.h>
struct DiskInfo GDiskInfo;
int GCurrentDisk;



void InitDisk(int id)
{
	//FLOPPY 1.44
	GDiskInfo.SizeSector = 512;
	GDiskInfo.CntSector = 18;
	GDiskInfo.CntCylinder = 80;
	GDiskInfo.CntHead = 2;
	GCurrentDisk = id;
}