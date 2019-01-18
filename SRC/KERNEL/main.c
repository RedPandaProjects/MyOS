#include <STDIO.h>
#include <STDLIB.h>
#include <BIOS.h>
#include <STRING.h>
#include <CONSOLE.h>
#include <KERNEL.h>
#include <MEMORY.h>
#include <DISK.h>
#include <FS/FS.h>
int get_build(const char*date, int start_year, int start_month, int start_day)
{
	static const char* month_id[12] =		 { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	int build_id = 0;
	int months = 0;
	int i = 0;
	static int days_in_month[12] =
	{
		31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
	

	for (i=0; i < 12; i++)
	{
		if (strncmp(date, month_id[i],3) != 0)
			continue;

		months = i;
		break;
	}

	build_id = (atoi(&date[7]) - start_year) * 365 + atoi(&date[4]) - start_day;

	for ( i = 0; i < months; ++i)
		build_id += days_in_month[i];

	for ( i = 0; i < start_month - 1; ++i)
		build_id -= days_in_month[i];
		
	return build_id;
}

int KernelMain(int drive)
{
	int c = 0,k=0;
	char *cmdLine = CmdLine;
	InitMemory();
	InitKernel();
	InitDisk(drive);
	printf("MyOS STARTUP...\r\n");
	printf("Build %d %s\r\n",get_build(__DATE__,2018,12,12),__DATE__);
	InitFS();
	printf(">");
	while(1)
	{
	
		c = bioskey(0x0);
		k = c >> 8;
		c = c & 0xFF;
		if (c >= 33 && c <= 127 || c==' ')
		{
			printf("%c", c);
			*cmdLine = c;
			cmdLine++;
		}
		else
		{
			switch (c)
			{
			case '\r':
			{
				*cmdLine = 0;
				printf("\r\n");
				if (ConsoleCall(CmdLine) == CONSOLE_NOT_FOUNT)
				{
					printf("Command [%s] don't found!!!\r\n", CmdLine);
				}
				printf(">");
				cmdLine = CmdLine;
				cmdLine[0] = 0;
			}
				break;
			case '\b':
				if (cmdLine != CmdLine)
				{
					cmdLine--;
					*cmdLine = 0;
					printf("\b \b");
				}
				break;
			default:
				break;
			}
		
		}
	}
	return 0;
}
