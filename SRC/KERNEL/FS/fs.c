#include <FS\FS.h>
#include <FS\FAT\FAT.h>
#include <ABORT.h>
#include <DISK.h>
#include "STDIO.h"
#include "CONSOLE.h"
#include <STRING.h>
FSFunctions GFSFunctions;
void InitFS()
{
	abort(InitFAT(),"unkown file system!!!", ERROR_DISK_UNKOWN_FILE_SYSTEM,GCurrentDisk,0,0,0);
}
int Console_DIR()
{
	FSItem item;
	char Name[15];
	if (!GFSFunctions.GetItem(&item))return 0;
	do
	{
		memset(Name, 0x20, 14);
		Name[14] = 0;
		memcpy(Name,item.Name,strlen(item.Name));
		printf("\t\t\t%s", Name);
		if (item.IsDir)
		{
			printf(" [DIR]");
		}
		else
		{	
			printf(" %lu", item.Size);

		}
		printf("\r\n");
	} 
	while (GFSFunctions.NextItem(&item));
	return 0;
}
int Console_CD()
{
	if (Argc==1)
	{

		char FAR* path = strtok(Argv[0], "\\/");
		do
		{
			if (!GFSFunctions.OpenDir(path))
			{
				printf("Error not found dir %s\r\n", Argv[0]);
				return -1;
			}
			path = strtok(0, "\\/");
		}
		while (path);
	}
	return 0;
}
