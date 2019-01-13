#include <DISK.h>
#include <BIOS.h>
#include <CONSOLE.h>
#include <stdio.h>
#include <STDLIB.h>
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
// SectorID = x % SectorsPerTrack + 1
// HeadID = (x / SectorsPerTrack) % HeadCount
// TrackID = (x / SectorsPerTrack) / HeadCount
int ReadSector(unsigned long x, void FAR*out)
{
	int a1 = x / GDiskInfo.CntSector;
	return biosdisk(0x2, GCurrentDisk, a1%GDiskInfo.CntHead,  a1 / GDiskInfo.CntHead, x % GDiskInfo.CntSector + 1, 1, out);
}
int WriteSector(unsigned long x,const void FAR*out)
{
	int a1 = x / GDiskInfo.CntSector;
	return biosdisk(0x3, GCurrentDisk, a1%GDiskInfo.CntHead, a1 / GDiskInfo.CntHead, x % GDiskInfo.CntSector + 1, 1,(void FAR*) out);
}

#ifdef DISK_DEBUG
char TestBuffer[512];
int Console_DiskRead()
{
	if (Argc == 1)
	{
		unsigned long m = atoul(Argv[0]);
		int i = 0;
		if (!ReadSector(m, TestBuffer))
		{
			printf("ERROR:Invalid sector!!!\r\n");
			return-1;
		}
		for (; i < 512; i++)
		{
			printf("%hhx", TestBuffer[i]);
		}
		printf("\r\n");
		return 0;
	}
	return -1;
}
#endif
