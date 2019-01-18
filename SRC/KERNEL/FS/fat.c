#include <STDLIB.h>
#include <DISK.h>
#include <FS\FAT\FAT.h>
#include <FS\FAT\FAT12.h>
#include <FS\FAT\FATBlock.h>
#include <ABORT.h>
#include <STRING.h>
#include <STDIO.h>
#include <MEMORY.h>
#include <FS/FAT/FATDir.h>
FATBootBlock GFATBootBlock;
FATInfo GFATInfo;

void FAR*GFSSector=0;

int InitFAT()
{
#ifdef FAT_DEBUG
	unsigned int i = 0,t=0,d=0;
	FATEntries dir,test;
	abort(sizeof(FATBootBlock) == 0x3e, "Fat boot block is  invalid!!!", GCurrentDisk, sizeof(FATBootBlock), 0x3e, 0, 0);
	abort(sizeof(FATEntries) == 0x20+2, "Fat boot block is  invalid!!!", GCurrentDisk, sizeof(FATEntries)-2, 0x20, 0, 0);
#endif

	if(GFSSector)
		MemoryFree(GFSSector);

	GFSSector = MemoryAlloc(  GDiskInfo.SizeSector);
	abort(ReadSector(0, GFSSector),"Sector is invalid!!!",ERROR_DISK_INVALID_SECTOR,GCurrentDisk,0,0,0);
	memcpy(&GFATBootBlock, GFSSector,sizeof(GFATBootBlock));
	
	if (!InitFAT12())
	{
		MemoryFree(GFSSector);
		return 0;
	}

	abort(GFATBootBlock.BytesPerSector == GDiskInfo.SizeSector&&
		GFATBootBlock.SectorsPerTrack == GDiskInfo.CntSector&&
		GFATBootBlock.GeadsPerCylinder == GDiskInfo.CntHead
		, "FAT is invalid!!!", ERROR_DISK_INVALID_BOOT_DATA_FAT, GCurrentDisk, 0, 0, 0);

#ifdef FAT_DEBUG
	printf("FAT Init\r\n");
	//printf("--reserved Sector[%d]\r\n", GFATBootrSector);
#endif
	GFATInfo.BeginFAT = GFATBootBlock.ReservedSectors;
	GFATInfo.BeginRoot = GFATInfo.BeginFAT + (GFATBootBlock.NumberOfFATs*GFATBootBlock.SectorsPerFAT);
	GFATInfo.BeginData = GFATInfo.BeginRoot + ((GFATBootBlock.RootEntries*(sizeof(FATEntries)-2)+ GFATBootBlock.BytesPerSector -1)/ GFATBootBlock.BytesPerSector);
	GFATInfo.CntFat = (GFATBootBlock.TotalSectors - GFATInfo.BeginData) / GFATBootBlock.SectorsPerCluster;
	GFATInfo.CntEntriesInClyster = (GFATBootBlock.BytesPerSector / (sizeof(FATEntries) - 2))*GFATBootBlock.SectorsPerCluster;
#ifdef FAT_DEBUG
	printf("--begin fat[%lx]\r\n--begin fat[%lx]\r\n--begin fat[%lx]\r\n", GFATInfo.BeginFAT, GFATInfo.BeginRoot, GFATInfo.BeginData);
#endif
#ifdef FAT_DEBUG
	printf("--count fat:%d\r\n", GFATInfo.CntFat);
#endif
	if (GFATClyster)MemoryFree(GFATClyster);
	GFATClyster = MemoryAlloc(GFATBootBlock.SectorsPerCluster*GFATBootBlock.BytesPerSector);

	//GFATBlockFunctions.FreeBlocks(0x1);
	/*
	FATAllocItem(&dir);
	memcpy(dir.Name, "TEST   ", 4);
	memcpy(dir.Ext, "   ", 3);
	dir.Size = 0;

	dir.Cluster = GFATBlockFunctions.AllocBlock();
	dir.Flags = 0x10;
	FATSaveItem(&dir);
	FATReadClyster(dir.Cluster);
	memset(GFATClyster, 0x0, 512);
	FATWriteClyster(dir.Cluster);
	FATOpenDir(&dir);


	

	for (i = 0; i < 32; i++)
	{
		FATAllocItem(&test);
		memcpy(test.Name, "TESTa  ", 5);
		if(i<16)
			test.Name[4] += i;
		else
			test.Name[4] = i+'A'-16;
		memcpy(test.Ext, "BIN", 3);
		test.Size = 512;
		test.Cluster = GFATBlockFunctions.AllocBlock();
		test.Flags = 0x01;
		FATSaveItem(&test);

		FATReadClyster(test.Cluster);
		memset(GFATClyster, 0xAA+i, 512);
		FATWriteClyster(test.Cluster);
	}	
	FATGetItem(&test); 
	for (i = 0; i < 32; i++)
	{

		GFATBlockFunctions.FreeBlocks(test.Cluster);
		FATFreeItem(&test);
		FATNextItem(&test);
	}
	FATCloseDir();
	GFATBlockFunctions.FreeBlocks(dir.Cluster);
	FATFreeItem(&dir);*/
	return 1;
}

void ClearFAT()
{
	if (GFSSector)
		MemoryFree(GFSSector);
	if (GFATClyster)
		MemoryFree(GFATClyster);
	GFATClyster = 0;
	GFSSector = 0;
}
