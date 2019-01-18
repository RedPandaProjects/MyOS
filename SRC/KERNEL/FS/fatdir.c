#include <FS/FAT/FATDir.h>
#include <FS/FAT/FATBlock.h>
#include <ABORT.h>
#include <DISK.h>
#include <STRING.h>
#include <STDIO.h>
static int IsRootDir = 1;
static unsigned int RootDirSector = 0;
static unsigned int LDirClysterBegin;
static unsigned int LDirClysterCurrent;

static void ReadRootDir(unsigned int id)
{

	unsigned int SectorTemp = 0;
	if (GClysterID != 0)
	{
		RootDirSector = -1;
		GClysterID = 0;
	}
	SectorTemp = id * (sizeof(FATEntries) - 2) / GFATBootBlock.BytesPerSector;
	if (SectorTemp != RootDirSector)
	{
		RootDirSector = SectorTemp;
		abort(ReadSector(RootDirSector + GFATInfo.BeginRoot, GFATClyster), "Sector is invalid!!!", ERROR_DISK_INVALID_SECTOR, GCurrentDisk, RootDirSector + GFATInfo.BeginRoot, 0, 0);
	}
}

static void SaveRootDir()
{
	unsigned int SectorTemp = 0;
	abort(WriteSector(RootDirSector + GFATInfo.BeginRoot, GFATClyster), "Sector is invalid!!!", ERROR_DISK_INVALID_SECTOR, GCurrentDisk, RootDirSector + GFATInfo.BeginRoot, 0, 0);
}
static int SeekDir(unsigned int id)
{

	unsigned int clysters = id / GFATInfo.CntEntriesInClyster, i;
	unsigned int Current = LDirClysterBegin;
	for (i = 0; i < clysters; i++)
	{
		Current = GFATBlockFunctions.NextBlock(Current);
		if (Current == 0xFFFF)return 0;
	}
	if (Current != LDirClysterCurrent||Current!=GClysterID)
	{
		LDirClysterCurrent = Current;
		FATReadClyster(Current);

	}
	return 1;
}
static void SaveDir()
{
	FATWriteClyster(LDirClysterCurrent);
}
int  FATGetItem(FATEntries * item)
{

	unsigned int i = 0,id;
	if (IsRootDir)
	{
		for (;i < GFATBootBlock.RootEntries; i++)
		{
			ReadRootDir(i);
			id = i % (GFATBootBlock.BytesPerSector/(sizeof(FATEntries)-2));
			memcpy(item, (char FAR*)GFATClyster + (id*(sizeof(FATEntries) - 2)), (sizeof(FATEntries) - 2));
			if (item->Flags != 0&&item->Name[0]!= 0xe5)
			{
				item->ID = i;
				return 1;
			}
		}
	}
	else
	{
		for (; SeekDir(i); i++)
		{
			id = i % GFATInfo.CntEntriesInClyster ;
			memcpy(item, (char FAR*)GFATClyster + (id*(sizeof(FATEntries) - 2)), (sizeof(FATEntries) - 2));
			if (item->Flags != 0 && item->Name[0] != 0xe5)
			{
				item->ID = i;
				return 1;
			}
		}
	}
	return 0;
}

int  FATNextItem(FATEntries * item)
{
	unsigned int i = item->ID+1, id;
	if (IsRootDir)
	{
		for (; i < GFATBootBlock.RootEntries; i++)
		{
			ReadRootDir(i);
			id = i % (GFATBootBlock.BytesPerSector / (sizeof(FATEntries) - 2));
			memcpy(item, (char FAR*)GFATClyster + (id*(sizeof(FATEntries) - 2)), (sizeof(FATEntries) - 2));
			if (item->Flags != 0 && item->Name[0] != 0xe5)
			{
				item->ID = i;
				return 1;
			}
		}
	}
	else
	{
		for (; SeekDir(i); i++)
		{
		
			id = i % GFATInfo.CntEntriesInClyster ;
			memcpy(item, (char FAR*)GFATClyster + (id*(sizeof(FATEntries) - 2)), (sizeof(FATEntries) - 2));
			if (item->Flags != 0 && item->Name[0] != 0xe5)
			{
				item->ID = i;
				return 1;
			}
		}
	}
	return 0;
}

void  FATSaveItem(FATEntries * item)
{
	unsigned int i = item->ID, id;
	if (IsRootDir)
	{
		ReadRootDir(i);
		id = i % (GFATBootBlock.BytesPerSector / (sizeof(FATEntries) - 2));
		memcpy((char FAR*)GFATClyster + (id*(sizeof(FATEntries) - 2)), item, (sizeof(FATEntries) - 2));
		SaveRootDir();
	}
	else
	{
		if (SeekDir(i)==0)return;
		id = i % GFATInfo.CntEntriesInClyster ;
		memcpy((char FAR*)GFATClyster + (id*(sizeof(FATEntries) - 2)),item, (sizeof(FATEntries) - 2));
		SaveDir();
	}
	return;
}

int  FATAllocItem(FATEntries * item)
{
	unsigned int i = 0, id=0;
	unsigned int clyster = 0, t_clyster;
	if (IsRootDir)
	{
		for (; i < GFATBootBlock.RootEntries; i++)
		{
			ReadRootDir(i);
			id = i % (GFATBootBlock.BytesPerSector / (sizeof(FATEntries) - 2));
			memcpy(item, (char FAR*)GFATClyster + (id*(sizeof(FATEntries) - 2)), (sizeof(FATEntries) - 2));
			if (item->Flags == 0|| item->Name[0] == 0xe5)
			{
				item->ID = i;
				return 1;
			}
		}
	}
	else
	{
		for (;  SeekDir(i); i++)
		{
			id = i % GFATInfo.CntEntriesInClyster;
			memcpy(item, (char FAR*)GFATClyster + (id*(sizeof(FATEntries) - 2)), (sizeof(FATEntries) - 2));
			if (item->Flags == 0 || item->Name[0] == 0xe5)
			{
				item->ID = i;
				return 1;
			}
		}
		clyster = LDirClysterBegin;
		while (1)
		{
			t_clyster = GFATBlockFunctions.NextBlock(clyster);
			if (t_clyster == 0xFFFF)break;
			clyster = t_clyster;
		}
		clyster = GFATBlockFunctions.AllocNextBlock(clyster);
		if (clyster != 0xFFFF)
		{
			GClysterID = clyster;
			memset(GFATClyster, 0, GFATBootBlock.SectorsPerCluster*GFATBootBlock.BytesPerSector);
			FATWriteClyster(clyster);
			return FATAllocItem(item);
		}
	}
	return 0;
}

void  FATFreeItem(FATEntries * item)
{
	const char FAR*in = GFATClyster;
	unsigned int lost_clyster,clyster = 0,t_clyster,i;
	item->Name[0] = 0xE5;
	FATSaveItem(item);
	if (IsRootDir == 0)
	{
		while (1)
		{
			clyster = LDirClysterBegin;
			while (1)
			{
				t_clyster = GFATBlockFunctions.NextBlock(clyster);
				if (t_clyster == 0xFFFF)break;
				lost_clyster = clyster;
				clyster = t_clyster;
			}
			if (clyster == LDirClysterBegin)
			{
				return;
			}
			FATReadClyster(clyster);
			for (i = 0; i < GFATInfo.CntEntriesInClyster;i++) 
			{
				if (in[0] != 0xE5 && in[0xb] != 0)
				{
					return;
				}
				in += sizeof(FATEntries) - 2;
			}
			GFATBlockFunctions.CutNextBlocks(lost_clyster);
		}
	}
}

void FATOpenDir(FATEntries * item)
{
	IsRootDir = 0;
	LDirClysterBegin = item->Cluster;
	LDirClysterCurrent = 0xFFFF;
}

void FATCloseDir()
{
	IsRootDir = 1;
}
