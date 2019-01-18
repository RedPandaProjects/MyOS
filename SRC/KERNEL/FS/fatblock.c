#include <FS/FAT/FAT.h>
#include <FS/FAT/FATBlock.h>
#include <DISK.h>
#include <ABORT.h>
#include <STDIO.h>
unsigned int GFSFatSectorID=-1;
FATBlockFunctions GFATBlockFunctions;
unsigned int GClysterID = 0xFFFF;
void FAR*GFATClyster = 0;

int FATReadClyster(unsigned int id_)
{
	unsigned long id = id_, i = 0;;
	char FAR*out =(char FAR*) GFATClyster;
	if (GClysterID == id_)return 1;
	GClysterID = id_;
#ifdef FAT_DEBUG
	abort(id != 0 && id_ < GFATInfo.CntFat, "Invalid Adress clyster", ERROR_DEBUG,(int)(id_), (int)(GFATInfo.CntFat), 0, 0);
#endif
	id -= 2;
	id *= GFATBootBlock.SectorsPerCluster;
	for (; i < GFATBootBlock.SectorsPerCluster; i++)
	{
		if (ReadSector(GFATInfo.BeginData + id + i, out) == 0)
			return 0;
		out += GFATBootBlock.BytesPerSector;
	}
	return 1;
}

int FATWriteClyster(unsigned int id_)
{
	unsigned long id = id_, i = 0;;
	char FAR*out = (char FAR*) GFATClyster; 
#ifdef FAT_DEBUG
	abort(id != 0 && id < GFATInfo.CntFat, "Invalid Adress clyster", ERROR_DEBUG, id, GFATInfo.CntFat, 0, 0);
#endif
	id -= 2; id *= GFATBootBlock.SectorsPerCluster; 
	for (; i < GFATBootBlock.SectorsPerCluster; i++)
	{
		if (WriteSector(GFATInfo.BeginData + id + i, out) == 0)
			return 0;
		out += GFATBootBlock.BytesPerSector;
	}
	return 1;
}
