#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct
{
	char *dat; // data
	int len; // 長さ
	int cap; // キャパシティ
	int ecnum; // 文字エンコードも保持したいよなぁ。0でASCII、とか
} str;

str strnew(int cap)
{
	str s;
	s.dat = malloc(cap + 1);
	*s.dat = '\0';
	s.len = 0;
	s.cap = cap;
	e.ecum = 0;
}

str strset(const char *p)
{
	int len = 0;

	while(p[len]) len++;
	
	str s = strnew(len);

	for(int k = 0; k < len; k++) s.dat[k] = p[k];
	s.dat[len] = '\0';
	s.len = len;

	return s;
}

str strcat(str s1, str s2)
{
	str res = strnew(s1.len + s2.len);

	for(int i = 0; i < s1.len; i++) res.dat[i] = s1.dat[i];
	for(int i = 0; i < s2.len; i++) res.dat[s1.len + i] = s2.dat[i];
	res.dat[res.len] = '\0';

	return res;
}

const int strlen(str s)
{
	return s.len;
}

const char *strget(str s)
{
	return s.dat;
}

int strfree(str *s)
{
	free(s->dat);
	s->dat = NULL;
	s->len = s->cap = 0;

	return 0;
}

str strprintf(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	int size = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);

	str s = strnew(size);
	va_start(ap, fmt);
	vsnprintf(s.dat, size + 1, fmt, ap);
	va_end(ap);
	s.l = size;

	return s;
}

int main(void)
{
	str hi = strnew(1);
	hi = strset("Hello");
	hi = strcat(hi, "World!");

	return 0;
}
