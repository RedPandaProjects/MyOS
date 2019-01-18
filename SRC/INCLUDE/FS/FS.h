#ifndef FS_H
#define FS_H
#include <SYSTEM.h>
void InitFS();
typedef struct 
{
	unsigned int Size;
	unsigned int StartClyster;
	unsigned int IsDir;
} FSItem;
typedef struct 
{
	int (*DeleteFile)(const char FAR*name);
	int(*DeleteDir)(const char FAR*name);

	int(*ExistFile)(const char FAR*name);
	int(*ExistDir)(const char FAR*name);

	int(*OpenDir)(const char FAR*name);
	int(*GetItemFromName)(const char FAR*name, FSItem*info);

	int(*GetItem)(FSItem*info);
	int(*NextItem)(FSItem*info);

	void FAR*(*GetClusterPointer)();
	unsigned int(*GetClusterSize)();

	void(*ReadCluster)(unsigned int id);
	void(*WriteCluster)(unsigned int id);
	unsigned int (*AllocNextCluster)(unsigned int id);
	unsigned int(*AllocCluster)();
	unsigned int(*NextCluster)(unsigned int id);

} FSFunctions;
extern FSFunctions GFSFunctions;
#endif
