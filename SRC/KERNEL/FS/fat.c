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
#include <FS/FS.h>
FATBootBlock GFATBootBlock;
FATInfo GFATInfo;

void FAR*GFSSector=0;
static void FATInitFunctions();
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
		ClearFAT();
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
	
	FATInitFunctions();

	//GFSFunctions.OpenDir

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
static int FAT_OpenDir(const char FAR*name)
{
	
	FATEntries dir;
	char temp[14];
	char FAR*ext = 0;
	char FAR*ext_=0;
	if (strlen(name) == 0)return 1;
	if (!FATGetItem(&dir))return 0;

	strcpy(temp, name);
	if (strcmp(temp, ".") != 0 && strcmp(temp, "..") != 0)
	{
		ext = strchr(temp, '.');
		if (ext)
		{
			*ext = 0;
			ext++;
		}
	}
	do
	{
		{
			ext_ = memchr(dir.Name, 0x20, 8);
			if (ext_)*ext_ = 0;

		}
		{
			ext_ = memchr(dir.Ext, 0x20, 3);
			if (ext_)*ext_ = 0;
		}
		if (dir.Flags&0x10&&strncmp(dir.Name, name, 8)==0)
		{
			if (!ext || strncmp(dir.Ext, ext, 3) == 0)
			{
				FATOpenDir(&dir);
				return 1;
			}
		}
	} while (FATNextItem(&dir));

	return 0;
}
static void FAT_ToRootDir()
{
	FATCloseDir();
}

static int FAT_GetIten(FSItem*item)
{
	char FAR*ext = 0;
	FATEntries e;
	if (!FATGetItem(&e))return 0;
	do
	{
		if (e.Flags & 0x20 || e.Flags & 0x10)
		{
			strncpy(item->Name, e.Name, 8);
			ext = strchr(item->Name, ' ');
			if (ext)*ext = 0;
			if (e.Ext[0] && e.Ext[0] != 0x20)
			{
				strcat(item->Name, ".");
				strncat(item->Name, e.Ext, 3);
			}
			item->ID = e.ID;
			item->Size = e.Size;
			item->IsDir = !!(e.Flags & 0x10);
			return 1;
		}
	} while (FATNextItem(&e));
	return 0;
}
static int FAT_NextItem(FSItem*item)
{
	char FAR*ext = 0;
	FATEntries e;
	e.ID = item->ID;
	if (!FATNextItem(&e))return 0;
	do
	{
		if (e.Flags & 0x20 || e.Flags & 0x10)
		{
			strncpy(item->Name, e.Name, 8);
			ext = strchr(item->Name, ' ');
			if (ext)*ext = 0;
			if (e.Ext[0] && e.Ext[0] != 0x20)
			{
				strcat(item->Name, ".");
				strncat(item->Name, e.Ext, 3);
			}
		//	item->Name[8] = 0;
			item->ID = e.ID;
			item->Size = e.Size;
			item->IsDir = !!(e.Flags & 0x10);
			return 1;
		}
	} while (FATNextItem(&e));
	return 0;
}
static void FATInitFunctions()
{
	GFSFunctions.OpenDir = FAT_OpenDir;
	GFSFunctions.ToRootDir = FAT_ToRootDir;
	GFSFunctions.GetItem = FAT_GetIten;
	GFSFunctions.NextItem = FAT_NextItem;
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
