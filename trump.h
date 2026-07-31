#ifndef TRUMP_H
#define TRUMP_H

#include <stdlib.h>
#include <time.h>

typedef unsigned char chr;
typedef unsigned int uint;

typedef struct
{	chr a;
	chr b;
} wrd;

chr card[54] = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f !\"#$%&'()*+,-./012345";

// C < D < H < S
// C = Club, D = Diamond, H = Heart, S = Spade

chr winit(wrd *w, chr len);
chr rinit(void);
wrd chr2wrd(chr rid);
chr wrd2chr(wrd w);
chr getSid(wrd w);
chr getCid(wrd w);
chr getSch(wrd w);
chr getCch(wrd w);
wrd suffle(wrd *list,  chr len);

chr winit(wrd *w, chr len)
{	if(!w)
		return 1;

	for(chr i = 0; i < len; i++)
		w[i] = chr2wrd(55);

	return 0;
}

chr rinit(void)
{	srand((uint)time(NULL));

	return 0;
}

wrd chr2wrd(chr rid)
{	wrd w;

	w.a = rid / 13;
	w.b = rid % 13;

	return w;
}

chr wrd2chr(wrd w)
{	return 13 * w.a + w.b;
}

chr getSid(wrd w)
{	return w.a;
}

chr getCid(wrd w)
{	return w.b;
}

chr getSch(wrd w)
{	switch(getSid(w))
	{	case 0:
			return 'c';

		case 1:
			return 'd';

		case 2:
			return 'h';

		case 3:
			return 's';

		case 4:
			return 'r';

		default:
			return 1;
	}
}

chr getCch(wrd w)
{	if(getSid(w) < 4)
	{	switch(getCid(w))
		{	case 0:
				return 'A';
	
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				return 0x31 + getCid(w);
	
			case 9:
				return 'X';
	
			case 10:
				return 'J';
	
			case 11:
				return 'Q';
	
			case 12:
				return 'K';

			default:
				return 1;
		}
	} else
	{	if(getCid(w) == 0)
			return '0';

		else if(getCid(w) == 1)
			return '1';

		else
			return 2;
	}
}

wrd suffle(wrd *list, chr len)
{	wrd w;
	chr f = 0;
	chr x = 0;
	chr i = 0;

	if(!list)
	{	w = chr2wrd(rand() % 54);

		return w;
	}

	while(1)
	{	w = chr2wrd(rand() % 54);

		for(i = 0; i < len; i++)
		{	if(wrd2chr(list[i]) == wrd2chr(w))
				break;
		} if(len <= i) return w;

		if(100 * len < x)
		{	w = chr2wrd(56);

			return w;
		} x++;
	}
}

#endif /* trump.h */
