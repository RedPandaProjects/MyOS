#include <string.h>
#include <CTYPE.h>

#pragma aux memcpy_ = \
	"cld"\
	"push ds"\
	"mov ds,dx"\
	"rep movsb"\
	"pop ds"\
modify [di]\  
parm[es di ][dx si ][cx];

void memcpy_(void FAR* dest, const void FAR* src, int n);

void FAR* memcpy(void FAR* dest, const void FAR* src, int n)
{
	 memcpy_(dest,src,n);
	 return dest;
}
#pragma aux memmove_right = \
	"push ds"\
	"mov ds,dx"\
	"add si,cx"\
	"add di,cx"\
	"dec si"\
	"dec di"\
	"std"\
	"rep movsb"\
	"cld"\
	"pop ds"\
modify [si di cx]\  
parm[es di][dx si][cx];
void memmove_right(void FAR* dest, const void FAR* src, int n);
void FAR* memmove(void FAR* dest, const void FAR* src, int n)
{
	if (dest > src)
	{
		memmove_right(dest, src, n);
	}
	else
	{
		memcpy(dest, src, n);
	}
	return dest;
}
#pragma aux memcmp_ = \
	"push ds"\
	"mov ds,dx"\
	"std"\
	"add si,cx"\
	"add di,cx"\
	"dec si"\
	"dec di"\
	"repz cmpsb"\
	"ja memcmp@1"\
	"jb memcmp@2"\
	"memcmp@0:"\
	"xor ax,ax"\
	"jmp memcmp@exit"\
	"memcmp@1:"\
	"mov ax,1"\
	"jmp memcmp@exit"\
	"memcmp@2:"\
	"mov ax,-1"\
	"memcmp@exit:"\
	"cld"\
	"pop ds"\
modify[si di cx]\
parm[dx si][es di][cx]\
value[ax]; 
int memcmp_(const void FAR* cs, const void FAR* ct, int count);
int memcmp(const void FAR* cs, const void FAR* ct, int count)
{
	return memcmp_(cs,ct,count);
}
#pragma aux memchr_ = \
	"cld"\
	"REPNE scasb"\
	"jz memchr@0"\
	"mov di,0"\
	"mov es,di"	\
	"jmp memchr@1"\
	"memchr@0:"\
	"dec di"\
	"memchr@1:"\
modify [cx es]     \  
parm[di es][ax][cx]\
value[di es];

void FAR* memchr_(const void FAR* cs, int c, int count);
void FAR* memchr(const void FAR* cs, int c, int count)
{
	return memchr_(cs,c,count);
}
#pragma aux memrchr_ = \
	"std"\
	"add di,cx"\
	"dec di"\
	"REPNE scasb"\
	"jz memchr@0"\
	"mov di,0"\
	"mov es,di"	\
	"jmp memchr@1"\
	"memchr@0:"\
	"inc di"\
	"memchr@1:"\
	"cld"\
modify [cx]\
parm[ es di][ax][cx]\
value[ es di];
void FAR* memrchr_(const void FAR* cs, int c, int count);
void FAR* memrchr(const void FAR* cs, int c, int count)
{
	return memrchr_(cs, c, count);
}

#pragma aux memset_ = \
	"cld"\
	"rep stosb" \
modify [di cx]  \  
parm[es di][ax][cx];
void  memset_(void FAR* s, int c, int count);
void FAR* memset(void FAR* s, int c,  int count)
{
	memset_(s,c,count);
	return s;
}

#pragma aux strlen_ = \
	"push ds"\
	"pop es"\
	"cld"\
	"mov bx,di"\
	"mov ax,0"\
	"mov cx,0xFFFF"\
	"REPNE scasb"\
	"sub di,bx"\
	"dec di"\
modify [ax bx cx]  \  
parm[ es di] \
value[di];

int strlen_(const char FAR* s);
int strlen(const char FAR* s)
{
	if (s==0)return 0;
	return strlen_(s);
}
char FAR* ___strtok;
char FAR* ___strtok_end;
char FAR* strtok(char FAR* s, const char FAR* ct)
{
	if (s)
	{
		while (*s && strpbrk(s, ct)) s++;
		___strtok = s;
		___strtok_end = s + strlen(s) + 1;
		while (*s)
		{
			while (*s && strpbrk(s, ct) == 0)s++;
			if (*s) 
			{
				*s = 0; 
				s++;
			}
			while (*s && strpbrk(s, ct)) s++;
		}
		return ___strtok;
	}
	else
	{
		if (___strtok != ___strtok_end)___strtok += strlen(___strtok);
		if (___strtok != ___strtok_end)___strtok++;
		while (___strtok != ___strtok_end && *___strtok&&strpbrk(___strtok, ct))___strtok++;
		if (___strtok == ___strtok_end)return 0;
		return ___strtok;
	}
}

char FAR* strcpy(char FAR* dest, const char FAR* src)
{
	return memcpy(dest, src, strlen(src) + 1);
}

char FAR* strncpy(char FAR* dest, const char FAR* src, int count)
{
	if (count  > strlen(src) )
	{
		count = strlen(src);
	}
	dest[count] = 0;
	return 	memcpy(dest, src, count);
}

char FAR* strcat(char FAR* dest, const char FAR* src)
{
	return memcpy(dest+ strlen(dest), src, strlen(src) + 1);
}

char FAR* strncat(char FAR* dest, const char FAR* src, int count)
{
	if (count > strlen(src) )
	{
		count = strlen(src);
	}
	dest[count + strlen(dest)] = 0;
	return memcpy(dest + strlen(dest), src, count);
}

int strcmp(const char FAR* cs, const char FAR* ct)
{
	int ccs = strlen(cs);
	int cct = strlen(ct);
	if (ccs == cct)
	{
		return memcmp(cs, ct,ccs);
	}
	else if (ccs > cct)
	{
		return 1;
	}
	else if (ccs < cct)
	{
		return -1;
	}
	return 0;
}

int strncmp(const char FAR* cs, const char FAR* ct, int count)
{
	int ccs = strlen(cs);
	int cct = strlen(ct);
	if (ccs > count)ccs = count;
	if (cct > count)cct = count;
	if (ccs == cct)
	{
		return memcmp(cs, ct, ccs);
	}
	else if (ccs > cct)
	{
		return 1;
	}
	else if (ccs < cct)
	{
		return -1;
	}
	return 0;
}

char FAR* strchr(const char FAR* s, int c)
{
	return memchr(s, c, strlen(s));
}

char FAR* strrchr(const char FAR* s, int c)
{
	return memrchr(s, c, strlen(s));
}

int strspn(const char FAR* cs, const char FAR* ct_)
{
	int result = 0;
	int count = 0;
	while (*cs)
	{
		if (strpbrk(cs, ct_))
		{
			count++;
		}
		else
		{
			if (count > result)result = count;
			count = 0;
		}
		cs++;
	}
	return result;
}

int strcspn(const char FAR* cs, const char FAR* ct)
{
	int result = 0;
	int count = 0;
	while (*cs)
	{
		if (strpbrk(cs, ct))
		{
			if (count > result)result = count;
			count = 0;
			
		}
		else
		{
			count++;
		}
		cs++;
	}
	return result;
}

char FAR* strpbrk(const char FAR* cs, const char FAR* ct)
{
	while (*ct)
	{
		if (*cs == *ct)
			return (char*)cs;
		ct++;
	}
	return NULL;
}
char FAR* strstr(const char FAR* cs, const char FAR* ct)
{
	cs = strchr(cs, ct[0]);
	while (cs)
	{
		if (strncmp(cs, ct, strlen(ct))==0)
			return (char*)cs;
		cs = strchr(cs+1, ct[0]);
	}
	return NULL;
}
