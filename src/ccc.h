#ifndef CCC_H
#define CCC_H

////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>

////////////////////////////////////////////////////////

#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7

#define BOLD 1
#define ITALIC 2
#define ULINE 3
#define FLUSH 4
#define SWITCH 5

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

int kbhit(void);
int csloff(int);

#endif
