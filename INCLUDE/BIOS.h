#ifndef BIOS_H
#define BIOS_H
#include <TYPE.h>
int biosdisk(int cmd, int drive, int head, int track, int sector,int nsects, void FAR*buffer);
#endif
