#include "ccc.h"
////////////////////////////////////////////////////////

struct termios origin;
int mode = 0;

////////////////////////////////////////////////////////

int cls(void);

int setmode(int);

int setcclr(int);
int setbclr(int);
int setstyle(int);

int resettxt(int);

int getrow(void);
int getcol(void);
int getposx(void);
int getposy(void);

int acs(int, int);
int rcs(int, int);

int bprint(const char *text, ...);
int bprintln(const char *text, ...);

int box(int, int, int, int);

int kbhit(void);
int csloff(int);

////////////////////////////////////////////////////////

int cls(void)
{
	printf("\x1b[2J");
	return 0;
}

int setmode(int set)
{
	static int inited = 0;

	if(set)
	{
		if(!inited)
		{
			tcgetattr(STDIN_FILENO, &origin);
			inited = 1;
		}

		struct termios new = origin;
		new.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &new);
		mode = 1;
	}
	else
	{
		if(inited)
		{
			tcsetattr(STDIN_FILENO, TCSANOW, &origin);
			mode = 0;
		}
	}

	return 0;
}

int setcclr(int cc)
{
	if((0 <= cc) && (cc < 8))
	{
		printf("\x1b[3%dm", cc);
	}

	return 0;
}

int setbclr(int cc)
{
	if((0 <= cc) && (cc < 8))
	{
		printf("\x1b[4%dm", cc);
	}

	return 0;
}

int setstyle(int sc)
{
	switch(sc)
	{
		case 0:
			break;

		case 1: // Bold
			printf("\x1b[1m");
			break;

		case 2: // Italic
			printf("\x1b[3m");
			break;

		case 3: // Under line
			printf("\x1b[4m");
			break;

		case 4: // flush
			printf("\x1b[5m");
			break;

		case 5: // Switch f/b color
			printf("\x1b[7m");
			break;
	}
	return 0;
}

int resettxt(int mode)
{
	switch(mode)
	{
		case 0:
			printf("\x1b[0m");
			break;

		case 1:
			printf("\x1b[39m");
			break;

		case 2:
			printf("\x1b[49m");
			break;
	}

	return 0;
}

int getrow(void)
{
	struct winsize ws;

	if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
	{
		return -1;
	}

	return ws.ws_row;
}

int getcol(void)
{
	struct winsize ws;

	if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
	{
		return -1;
	}

	return ws.ws_col;
}

int getposx(void)
{
	struct timeval tv = {0, 200000};
	char buf[32];
	int col = -1;
	fd_set fds;

	for(int i = 0; i < 3; i++)
	{
		write(STDOUT_FILENO, "\x1b[6n", 4);

		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO, &fds);

		if(select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) <= 0)
		{
			continue;
		}

		int len = read(STDIN_FILENO, buf, sizeof(buf) - 1);
		if(len <= 0)
		{
			continue;
		}

		buf[len] = 0;

		int row = -1;
		if(sscanf(buf, "\x1b[%d;%dR", &row, &col) == 2)
		{
			return col - 1;
		}
	}

	return -1;
}

int getposy(void)
{
	struct timeval tv = {0, 200000};
	char buf[32];
	int row = -1;
	fd_set fds;

	for(int i = 0; i < 3; i++)
	{
		write(STDOUT_FILENO, "\x1b[6n", 4);

		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO, &fds);

		if(select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) <= 0)
		{
			continue;
		}

		int len = read(STDIN_FILENO, buf, sizeof(buf) - 1);
		if(len <= 0)
		{
			continue;
		}

		buf[len] = 0;

		int col = -1;
		if(sscanf(buf, "\x1b[%d;%dR", &row, &col) == 2)
		{
			return row - 1;
		}
	}

	return -1;
}

int acs(int x, int y)
{
	if(((x < 0) || (getcol() < x)) || ((y < 0) || (getrow() < y)))
	{
		return 1;
	}
	printf("\x1b[%d;%dH", y + 1, x + 1);
	fflush(stdout);
	return 0;
}

int rcs(int dx, int dy)
{
	int x = getposx();
	int y = getposy();
	if((x < 0) || (y < 0))
	{
		return 1;
	}
	else
	{
		int val = acs(x + dx, y + dy);
		fflush(stdout);
		return val;
	}
}

int bprint(const char *text, ...)
{
	int bakx = getposx();
	int baky = getposy();

	va_list ap;
	va_start(ap, text);
	vprintf(text, ap);
	va_end(ap);

	acs(bakx, baky);
	fflush(stdout);

	return 0;
}

int bprintln(const char *text, ...)
{
	int bakx = getposx();
	int baky = getposy();

	va_list ap;
	va_start(ap, text);
	vprintf(text, ap);
	va_end(ap);

	acs(bakx, baky + 1);

	return 0;
}

int box(int sx, int sy, int ex, int ey)
{
	int bx = getposx();
	int by = getposy();

	acs(sx, sy);

	bprintln("+");

	for(int i = 0; i < ex - 1; i++)
	{
		bprintln("|");
	}

	bprint("+");

	acs(sx + 1, sy);

	for(int i = 0; i < ey - 1; i++)
	{
		printf("-");
	}

	printf("+");

	acs(ex, sy + 1);

	for(int i = 0; i < ex - 1; i++)
	{
		bprintln("|");
	}

	acs(sx + 1, ey);

	for(int i = 0; i < ey - 1; i++)
	{
		printf("-");
	}

    printf("+");

	acs(bx, by);

	return 0;
}

int kbhit(void)
{
	int res = 0;
	if(mode)
	{
		fd_set fds;
		struct timeval tv;

		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO, &fds);

		tv.tv_sec = 0;
		tv.tv_usec = 0;

		res = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
	}

	return res > 0;
}

int csloff(int mode)
{
	if(mode)
	{
		printf("\x1b[?25l");
	}
	else
	{
		printf("\x1b[?25h");
	}

	return 0;
}
