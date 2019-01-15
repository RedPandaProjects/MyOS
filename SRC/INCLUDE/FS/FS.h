#ifndef FS_H
#define FS_H
#include <SYSTEM.h>
void InitFS();
typedef struct 
{
	int size;
	int cntSector;
	int startSector;
	int isDir;
} FSItem;

struct FSFunctions
{
	int (*deleteFile)(const char FAR*name);
	int(*deleteDir)(const char FAR*name);

	int(*existFile)(const char FAR*name);
	int(*existDir)(const char FAR*name);

	int(*openDir)(const char FAR*name);
	int(*getFile)(const char FAR*name, FSItem*info);

	int(*getItem)(FSItem*info);
	int(*nextItem)(FSItem*info);


	int(*readSector)(int sector,void FAR*out);
	int(*writeSector)(int sector,const void FAR*out);
	int(*nextSector)(int sector);
	int(*allocSector)(int sector);

} F_FS;
#endif
