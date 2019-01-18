#ifndef ABORT_H
#define ABORT_H
void abort(const char*name,unsigned int id, unsigned int a, unsigned int  b, unsigned int  c, unsigned int  d);
#define abort(i,name,id,a,b,c,d) if((i)==0)abort(name,id,a,b,c,d);
#define ERROR_DISK_INVALID_SECTOR 0x01
#define ERROR_DISK_INVALID_BOOT_DATA_FAT 0x02
#define ERROR_DISK_INVALID_FAT 0x03
#define ERROR_DISK_UNKOWN_FILE_SYSTEM 0x04
#define ERROR_DEBUG 0xFFFF
#endif
