#include <FS/FAT/FAT12.h>
#include <FS/FAT/FAT.h>
#include <FS/FAT/FATBlock.h>
#include <DISK.h>
#include <string.h>
#include <ABORT.h>
#include <STDIO.h>
#undef FAT_12_DEBUG
static void FAT12_ReadSector(unsigned long id)
{

	unsigned long  i = 0;
	for (;(unsigned char)i < GFATBootBlock.NumberOfFATs; i++)
	{
		if (ReadSector(GFATInfo.BeginFAT + (id + (i*GFATBootBlock.SectorsPerFAT)), GFSSector)) { GFSFatSectorID = id; return; }
	}
	
	abort(0, "file allocator table is dead!!!", ERROR_DISK_INVALID_FAT, GCurrentDisk, id,0x1,0x12);
}
static void FAT12_WriteSector(unsigned long id)
{
	int isWrited = 0;
	unsigned long i = 0;
	for (; (unsigned char)i < GFATBootBlock.NumberOfFATs; i++)
	{
		if (WriteSector(GFATInfo.BeginFAT + (id + (i*GFATBootBlock.SectorsPerFAT)), (char FAR*)GFSSector))
		{
			isWrited = 1;
		}
	}
	abort(isWrited, "file allocator table is dead!!!", ERROR_DISK_INVALID_FAT, GCurrentDisk, id, 0x2, 0x12);
}

static int FAT12_ReadWord(unsigned int id)
{

	unsigned char FAR* out_buf =(unsigned char FAR* ) GFSSector;
	unsigned int out = 0;
	unsigned int sector =	id / GFATBootBlock.BytesPerSector;
	unsigned int seek =		id % GFATBootBlock.BytesPerSector;
	if (GFSFatSectorID != sector)
	{
		FAT12_ReadSector(sector);
	}
	if (seek + 2 > GFATBootBlock.BytesPerSector)
	{
		out = *(unsigned int FAR*)(out_buf + seek-1);
		out = out >> 8;
		FAT12_ReadSector(sector+1);
		out = out |((*(unsigned int FAR*)(out_buf))<<8);
#ifdef FAT_12_DEBUG
//		printf("fat12:Read word:%x[%x]\r\n", id, out);
#endif
	}
	else
	{
		return *(unsigned int FAR*)(out_buf + seek);

	}
	return out;
}
static void FAT12_WriteWord(unsigned int id, unsigned int block)
{
	char FAR* out_buf = (unsigned char FAR*) GFSSector;
	unsigned int out = 0;
	unsigned int sector = id / GFATBootBlock.BytesPerSector;
	unsigned int seek = id % GFATBootBlock.BytesPerSector;
	if (GFSFatSectorID != sector)
	{
		FAT12_ReadSector(sector);
	}
	if (seek + 2 > GFATBootBlock.BytesPerSector)
	{
		*(unsigned char FAR*)(out_buf + seek) = block &0xFF;
		FAT12_WriteSector(sector);
		FAT12_ReadSector(sector + 1);
		*(unsigned char FAR*)(out_buf ) = block >>8;
		FAT12_WriteSector(sector+1);
#ifdef FAT_12_DEBUG
		printf("fat12:write word:%x[%x]\r\n", id, block);
#endif
	}
	else
	{
		*(unsigned int FAR*)(out_buf + seek) = block;
		FAT12_WriteSector(sector);
	}

}
static void FAT12_WriteBlock(unsigned int id, unsigned int data)
{

	unsigned int temp = 0;
	unsigned int seek = 0;
	unsigned int left = 0;

	seek = (id * 3) / 2;
	left = (id * 3) % 2;
	temp = FAT12_ReadWord(seek);
#ifdef FAT_12_DEBUG
//	printf("(%x)[%d]", temp, left);
#endif

	if (left)
	{
		temp &= 0x000F;
		data <<= 4;;
		data &= 0xFFF0;
		temp |= data;
	}
	else
	{
		temp &= 0xF000;
		data &= 0x0FFF;
		temp |= data;
	
	}
#ifdef FAT_12_DEBUG
	//printf("[%x]", temp);
#endif
	FAT12_WriteWord(seek, temp);
}

static unsigned int FAT12_ReadBlock(unsigned int id)
{

	unsigned int data = 0;
	unsigned int seek = 0;
	unsigned int left = 0;
#ifdef FAT_12_DEBUG
	//printf("fat12:Read block:%u\r\n", id);
#endif
	seek = (id * 3) / 2;
	left = (id * 3) % 2;
	data = FAT12_ReadWord(seek);
	data >>= left * 4;
	data &= 0x0fff;
	if (data == 0x0fff)return 0xffff;
#ifdef FAT_12_DEBUG
	//printf("fat12:Read block:%u\r\n", data);
#endif
	return data;
}

static unsigned int FAT12_AllocBlock()
{
	unsigned int i = 1;
	for (;i <= GFATInfo.CntFat; i++)
	{
		if (FAT12_ReadBlock(i)==0x0)
		{
			FAT12_WriteBlock(i, 0xFFFF);
			return i;
		}
	}
	return 0xFFFF;
}
static unsigned int FAT12_AllocNextBlock(unsigned int id)
{
	unsigned int i = 0;
	for (; i <= GFATInfo.CntFat; i++)
	{
		if (id!=i&&FAT12_ReadBlock(i)==0x0)
		{

			FAT12_WriteBlock(id,i);
			FAT12_WriteBlock(i, 0xFFFF);
			return i;
		}
	}
	return 0xFFFF;
}
static unsigned int FAT12_NextBlock(unsigned int id)
{
	return FAT12_ReadBlock(id);
}
static void FAT12_FreeBlocks(unsigned int i)
{
	unsigned int temp = 0;
	while (i != 0xFFFF)
	{

		temp = FAT12_ReadBlock(i);
		FAT12_WriteBlock(i, 0x0);
		i = temp;
	}
}
static void FAT12_CutBlocks(unsigned int i)
{
	unsigned int temp = 0;
	temp = FAT12_ReadBlock(i);
	FAT12_WriteBlock(i, 0xFFFF);
	i = temp;
	
	while (i != 0xFFFF)
	{

		temp = FAT12_ReadBlock(i);
		FAT12_WriteBlock(i, 0x0);
		i = temp;
	}
}
int InitFAT12()
{
	if (memcmp(GFATBootBlock.FileSystem, "FAT12 ", 6) == 0)
	{

		GFATBlockFunctions.AllocBlock = FAT12_AllocBlock;
		GFATBlockFunctions.AllocNextBlock= FAT12_AllocNextBlock;
		GFATBlockFunctions.NextBlock = FAT12_NextBlock;
		GFATBlockFunctions.FreeBlocks = FAT12_FreeBlocks;
		GFATBlockFunctions.CutNextBlocks = FAT12_CutBlocks;
		return 1;
	}
	return 0;
}
