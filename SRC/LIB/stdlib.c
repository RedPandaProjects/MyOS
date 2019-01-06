#include <STDLIB.h>
#include <INTERS.h>
#include <CTYPE.h>
int atoi(const char * text)
{
	int result = 0;
	if (text == 0)return 0;
	while (isspace(*text))text++;
	while (*text&&*text >= '0'&&*text <= '9')
	{
		result = result * 10;
		result += *text - '0';
		text++;
	}
	return result;
}

unsigned long atoul(const char * text)
{
	unsigned long result = 0;
	if (text == 0)return 0;
	while (isspace(*text))text++;
	while (*text&&*text >= '0'&&*text <= '9')
	{
		result = result * 10;
		result += *text - '0';
		text++;
	}
	return result;
}

int system(const char * text)
{
	return INTR_CALL(0, (int)text,0,0, get_ds());
}
