#ifndef BIOS_H
#define BIOS_H
#include <SYSTEM.h>
int biosdisk(int cmd, int drive, int head, int track, int sector,int nsects, void FAR*buffer);
int bioskey(int cmd);
int biosprint(int cmd, int byte, int port);
#endif
