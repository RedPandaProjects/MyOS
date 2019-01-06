#ifndef CONSOLE_H
#define CONSOLE_H
#include <SYSTEM.h>
	#define CONSOLE_NOT_FOUNT 0x0AAA
	#define CONSOLE_EMPTY (0x0ADD)
	int ConsoleCall(const char FAR*cmd);
	typedef struct
	{
		const char*Name;
		int(*Function)();
	} ConsoleCommand;
	extern int   Argc;
	extern char* Argv[256];
	extern char CmdLine[512];
#endif
