#include <MEMORY.h>
#include <STRING.h>
#include <STDIO.h>
#include <CONSOLE.h>
#include <STDLIB.h>
char MemBlocks[256];
void InitMemory()
{
	memset(MemBlocks, 0, sizeof(MemBlocks));
} 
int MemoryStatus()  
{
	int i, a,cnt; 
	int offset = 0;
	int size = 512;
	for (i = 0; i < 256; i++)
	{
		if (MemBlocks[i] != MEM_BLOCK_FREE)
		{
			size -= 2;
		}
	}
	printf("Mem Use:%d KB\r\nMem Free:%d KB\r\n", 512 - size, size);
	return 0;
}

void FAR* MemoryAlloc(unsigned long size)
{

	int cnt = (int)((size + 2047) / 2048);
	int cnt_free = 0, i = 0,offset=0;
	for (i = 0; i < 256; i++)
	{ 
		if (MemBlocks[i] == MEM_BLOCK_FREE)
		{
			cnt_free++;
			if (cnt == cnt_free)
			{
				memset(&MemBlocks[offset], MEM_BLOCK, cnt);
				MemBlocks[offset] = MEM_BLOCK_START;
				return (void FAR*)(((unsigned long)(128 * 0x40) + (offset * 0x80))<<16);
			}
		}
		else
		{
			offset = i+1;
			cnt_free = 0;

		}
	}
	return NULL;
}

void MemoryFree(void FAR* ptr)
{

	int i;
	int  block = (unsigned int)((unsigned long)ptr>>16);
	if(ptr == 0)return;
	block -= 128 * 0x40;
	block /= 0x80;
	if (MemBlocks[block] == MEM_BLOCK_START)
	{
		MemBlocks[block] = MEM_BLOCK;
		for ( i = block; i < 256; i++)
		{
			if (MemBlocks[i] != MEM_BLOCK)
				break;
			MemBlocks[i] = MEM_BLOCK_FREE;
		}
	}
}
#ifdef MEM_DEBUG
static void FAR*Console_Mem=0;
static char FAR*Console_Mem_Cur = 0;
int Console_MemoryAlloc()
{
	MemoryFree(Console_Mem);

	if (Argc > 0)
	{
		unsigned long m = atoul(Argv[0]);
		if (m == 0)return -1;
		Console_Mem = MemoryAlloc(m);
		memset(Console_Mem, 0xCB, (int)m);
	}
	return 0;
}
int Console_MemoryFree()
{
	if (Console_Mem == 0)return 0;
	Console_Mem_Cur = 0;
	MemoryFree(Console_Mem);
	Console_Mem = 0;
	return 0;
}

int Console_MemoryStatus()
{
	int i = 0;
	for (; i < 256; i++)
	{
		printf("%d",MemBlocks[i]);
	}
	printf("\r\n");
	return 0;
}

int Console_MemorySet()
{

	char byte;
	int i = 0;
	if (Console_Mem_Cur == 0)Console_Mem_Cur = Console_Mem;
	if (Console_Mem == 0)
	{
		printf("error:memory not alloc\r\n");
		return -1;
	}
	if (Argc)
	{
		for (i = 0; i < Argc; i++)
		{
			if (strlen(Argv[i]) != 2)
			{
				printf("error:invalid argument %s\r\n", Argv[i]);
				return -1;
			}

			if (Argv[i][0] >= '0'&&Argv[i][0] <= '9')
			{
				byte = (Argv[i][0] - '0')<<4;
			}
			else if (Argv[i][0] >= 'A'&&Argv[i][0] <= 'F')
			{
				byte = (Argv[i][0] - 'A' + 10) << 4;
			}
			else if (Argv[i][0] >= 'a'&&Argv[i][0] <= 'f')
			{
				byte = (Argv[i][0] - 'a' + 10) << 4;
			}
			else
			{
				printf("error:invalid argument %s\r\n", Argv[i]);
				return -1;
			}

			if (Argv[i][1] >= '0'&&Argv[i][1] <= '9')
			{
				byte += (Argv[i][1] - '0');
			}
			else if (Argv[i][1] >= 'A'&&Argv[i][1] <= 'F')
			{
				byte += (Argv[i][1] - 'A' + 10);
			}
			else if (Argv[i][1] >= 'a'&&Argv[i][1] <= 'f')
			{
				byte += (Argv[i][1] - 'a' + 10) ;
			}
			else
			{
				printf("error:invalid argument %s\r\n", Argv[i]);
				return -1;
			}
			*Console_Mem_Cur = byte;
			Console_Mem_Cur++;
		}
	}
	else
	{
		printf("error:without arguments\r\n");
		return -1;
	}
	return 0;
}
#pragma aux Console_MemoryCall_ = \
	"push ds"\
\
	"push cs"\
	"call t"\
	"t:"\
\
	"pop bx"\
	"add bx,10"\
	"push bx"\
\
	"push es"\ 
	"pop ds"\
\
	"push es"\
	"push di"\
	"retf"\
\
	"pop ds"\
modify [ax dx bx cx es]\  
parm[es di];
void Console_MemoryCall_(void FAR*);
int Console_MemoryCall()
{
	if(Console_Mem)Console_MemoryCall_(Console_Mem);
	return 0;
}

int Console_MemoryTo()
{
	if (Argc > 0&&Console_Mem)
	{
		int m = atoi(Argv[0]);
		Console_Mem_Cur = (char FAR*)Console_Mem + m;
	}
	return 0;
}
#endif
