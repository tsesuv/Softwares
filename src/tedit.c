/* チューリング完全なラインエディタ「turing editor - tedit」 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

////////////////////////////////////////////////////////

typedef struct
{
	char *text;
	struct ldat *prev;
	struct ldat *next;
} ldat;

typedef struct
{
	ldat *head;
	ldat *tail;
	ldat *current;
	unsigned int *num;
} buf;

typedef int (*builtin)(unsigned int argc, ...);

typedef struct
{
	char *CmdName;
	char *CmdHelp;
	builtin CmdFunc;
} cmds;

////////////////////////////////////////////////////////

int ExecBuiltIn(char *, unsigned int, ...);
int tell(char *);
char *split(char *, char, int);
int _quit(unsigned int, ...);
int _edit(unsigned int, ...);

////////////////////////////////////////////////////////

cmds BuiltInCmdList[] =
{
	{"q", "?[TextClr=GREEN]No Argments.?[TextClr=Default]?,  Quit to the TEDIT.?,", _quit},
	{"e", "?[TextClr=GREEN]<LineNo>[TextClr=Default]?,  Edit to Current Line or <LineNo>.?,", _edit},
	{NULL, NULL, NULL}
};

////////////////////////////////////////////////////////

int main(void)
{
	tell("Hello, world!?,");
	tell("?[TextClr=RED]This is a test message![TextClr=Default]?,");
	return 0;
}

int ExecBuiltIn(char *cmd, unsigned int argc, ...)
{
	va_list ap;
	va_start(ap, argc);

	for (int i = 0; BuiltInCmdList[i].CmdName != NULL; i++)
	{
		if (strcmp(cmd, BuiltInCmdList[i].CmdName) == 0)
		{
			return BuiltInCmdList[i].CmdFunc(argc, ap);
		}
	}

	printf("ERR: TEDIT: SYSTEM: EXECBUILTIN: Unknown command '%s'.\n", cmd);
	va_end(ap);

	return -1;
}

////////////////////////////////////////////////////////

int tell(char *text)
{
	char *escape = (char *)malloc(sizeof(char));
	char *buffer = (char *)malloc(sizeof(char));
	if(!(escape && buffer))
	{
		free(escape);
		free(buffer);
		return -1;
	}
	int k = 0;
	int l = 0;
	for(int i = 0; text[i] != 0x1A; i++)
	{
		if(text[i] == '?')
		{
			escape[l] = text[i];

			if(strlen(escape) < l)
			{
				char *etmp = (char *)realloc(escape, sizeof(char) * (l + 1));
				if(!etmp)
				{
					free(escape);
					free(buffer);
					return -1;
				}
				escape = etmp;
			}

			if(text[i + 1] == ',')
			{
				printf("\n");
				break;
			}

			if(text[i] == ']')
			{
				escape[l + 1] = 0;
				if(strcmp(split(escape, '=', 0), "?[TextClr") == 0)
				{
					if(strcmp(split(escape, '=', 1), "Default") == 0)
					{
						printf("\x1b[30m");
					}
					else if(strcmp(split(escape, '=', 1), "RED") == 0)
					{
						printf("\x1b[31m");
					}
					else if(strcmp(split(escape, '=', 1), "GREEN") == 0)
					{
						printf("\x1b[32m");
					}
					else if(strcmp(split(escape, '=', 1), "YELLOW") == 0)
					{
						printf("\x1b[33m");
					}
					else if(strcmp(split(escape, '=', 1), "BLUE") == 0)
					{
						printf("\x1b[34m");
					}
					else if(strcmp(split(escape, '=', 1), "MAGENTA") == 0)
					{
						printf("\x1b[35m");
					}
					else if(strcmp(split(escape, '=', 1), "CYAN") == 0)
					{
						printf("\x1b[36m");
					}
					else if(strcmp(split(escape, '=', 1), "WHITE") == 0)
					{
						printf("\x1b[37m");
					}

					free(escape);
					break;
				}
			}
			l++;
		}
		else
		{
			if(strlen(buffer) < k)
			{
				char *tmp = (char *)realloc(buffer, sizeof(char) * (k + 1));
				if(!tmp)
				{
					free(buffer);
					return -1;
				}
				buffer = tmp;
			}

			buffer[k] = text[i];
			k++;
			l = 0;
		}
	}

	buffer[k] = 0;

	printf(buffer);

	free(buffer);

	return 0;
}

char *split(char *p, char dchar, int i)
{
	int k = 0;
	int n = 0;
	int l = 0;
	int memsize = 0;
	while(*(p + l) != '\0')
	{
		l++;
		memsize++;
	}
	char *res_list = (char *)malloc(memsize - 1);
	if(!res_list)
	{
		free(res_list);
		return NULL;
	}
	for(int m = 0; m < l; m++)
	{
		*(res_list + m) = 0;
	}
	char tmp[l + 2];

	for (int ti = 0; ti < l; ti++)
	{
		*(tmp + ti + 1) = *(p + ti);
	}
	*(tmp + l + 1) = dchar;
	*tmp = dchar;

	for (int j = 0; j < l + 2; j++)
	{
		if (*(tmp + j) == dchar)
		{
			k++;
			if (k - 1 == i)
			{
				j++;
				while (1)
				{
					if (k - 1 == i + 1)
					{
						return res_list;
					}
					if (*(tmp + j) == dchar)
					{
						k++;
					}
					else
					{
						*(res_list + n) = *(tmp + j);
					}
					n++;
					j++;
				}
			}
		}
	}
	return res_list;
}

int _quit(unsigned int argc, ...)
{
	va_list ap;
	va_start(ap, argc);

	if (argc > 0)
	{
		printf("ERR: TEDIT: BUILTIN: QUIT: Too many arguments.\n");
		va_end(ap);
		return -1;
	}

	va_end(ap);

	printf("TEDIT: SYSTEM: INFO: TEDIT - Turing Editor 1.0.0-PA\n");
	printf("TEDIT: QUIT: Exiting the TEDIT now...\n");

	exit(0);
}

int _edit(unsigned int argc, ...)
{
	va_list ap;
	va_start(ap, argc);
	
	va_end(ap);

	return 0;
}
