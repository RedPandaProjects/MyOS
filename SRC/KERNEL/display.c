#include <DISPLAY.h>
#include <BIOS.h>
int ClearScreen()
{
	biosprint(0x0, 0x03,0);
	return 0;
}
void PrintText(const char FAR*text, int size)
{
	while (size--) {
		biosprint(0x0E, *text, 0);
		text++;
	}
}
