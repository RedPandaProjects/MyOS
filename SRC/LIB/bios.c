#include <BIOS.h>

// drive:DL
// adress:DS:BX
// head:DH
// : -- - CH-- - -- - CL-- -
// track:76543210 98
// sector:            543210

#pragma aux _biosdisk_ = \
	"int 0x13"			 \
modify [dx cx bx es]     \  
parm[ax][dx][cx][bx es] \
value[ax];
int _biosdisk_(int ax, int dx, int cx, void FAR*buffer);
int biosdisk(int cmd, int drive, int head, int track, int sector, int nsects, void FAR*buffer)
{
	int cx = ((sector & (~0xC0)) | ((track & 0x300) >> 2))|(track << 8);
	int dx = (drive )|(head << 8);
	int ax =  (nsects)|(cmd << 8);
	return _biosdisk_(ax, dx, cx, buffer) == nsects;
}
#pragma aux _bioskey_ = \
	"int 0x16"\
parm[ax]\
value[ax];
int _bioskey_(int);
int bioskey(int cmd)
{
	return _bioskey_(cmd);
}
#pragma aux _biosprint_ = \
	"int 0x10"\
parm[ax][bx]\
value[ax];
int _biosprint_(int,int);
int biosprint(int cmd, int byte, int port)
{
	return _biosprint_(cmd<<8|byte,port);
}
