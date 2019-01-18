#include <FS\FS.h>
#include <FS\FAT\FAT.h>
#include <ABORT.h>
#include <DISK.h>
void InitFS()
{
	abort(InitFAT(),"unkown file system!!!", ERROR_DISK_UNKOWN_FILE_SYSTEM,GCurrentDisk,0,0,0);
}
