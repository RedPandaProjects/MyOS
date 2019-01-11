#include <STDIO.h>
#include <STRING.h>
#include "CONSOLE.h"
#include "DISPLAY.h"
#include "MEMORY.h"
int   Argc;
char* Argv[256];
int test()
{
	printf("hello world\r\n");
	return 0;
}
char CmdLine[512];
ConsoleCommand ConsoleCommands[]=
{
	{"test",test},
	{"cls",ClearScreen},
	{"mem",MemoryStatus},
#ifdef MEM_DEBUG
	{"mem_alloc",Console_MemoryAlloc},
	{"mem_free",Console_MemoryFree},
	{"mem_status",Console_MemoryStatus},
	{"mem_set",Console_MemorySet},
	{"mem_call",Console_MemoryCall},
	{"mem_to",Console_MemoryTo},
#endif
	{0,0}
};
int ConsoleCall(const char FAR* cmd)
{
	char *cmdLine = CmdLine;
	const char*command = 0;
	int i;
	if (CmdLine != cmd)
	{
		strcpy(CmdLine, cmd);
	}
	cmdLine = (char*)strtok(CmdLine, " \r\n\t");
	command = cmdLine;
	if (command == 0||*command==0)return CONSOLE_EMPTY;
	Argc = 0;

	cmdLine = (char*)strtok(0, " \r\n\t");
	while (cmdLine)
	{
		Argv[Argc++] = cmdLine;
		cmdLine = (char*)strtok(0, " \r\n\t");
	}


	for (i = 0;  ConsoleCommands[i].Name; i++)
	{
		if (strcmp(ConsoleCommands[i].Name, command) == 0)
		{
			return ConsoleCommands[i].Function();
		}
	}
	return CONSOLE_NOT_FOUNT;
}
