#ifndef FAT_H
#define FAT_H
#include <SYSTEM.h>
#define FAT_DEBUG
#pragma pack(push,1)
typedef struct
{
	char null[3];
	char Desc[8];
	unsigned int    BytesPerSector;
	unsigned char   SectorsPerCluster;
	unsigned int 	ReservedSectors;
	unsigned char	NumberOfFATs;
	unsigned int 	RootEntries;
	unsigned int 	TotalSectors;
	unsigned char	Media;
	unsigned int 	SectorsPerFAT;
	unsigned int 	SectorsPerTrack;
	unsigned int 	GeadsPerCylinder;
	char null2[8];
	unsigned char	 DriveNumber;
	char null3;
	unsigned char	Bootsig;
	unsigned long Serial;
	char VolumeLabel[11];
	char FileSystem[8];
} FATBootBlock;
#pragma pack(pop)
extern FATBootBlock GFATBootBlock;
typedef struct
{
	unsigned long BeginFAT;
	unsigned int  CntFat;
	unsigned long BeginRoot;
	unsigned long BeginData;
	unsigned int CntEntriesInClyster;

} FATInfo;
#pragma pack(push,1)
typedef struct
{
	char Name[8];
	char Ext[3];
	unsigned char Flags;
	char null[10];
	unsigned int Time;
	unsigned int Date;
	unsigned int Cluster;
	unsigned long Size;
	unsigned int ID;
} FATEntries;
#pragma pack(pop)
extern FATInfo GFATInfo;
extern void FAR*GFSSector;

int InitFAT();
void ClearFAT();
#endif
