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
wrd suffle(wrd *list,  chr *hLen, chr lLen);

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

wrd suffle(wrd *list, chr *hLen, chr lLen)
{	if(54 < lLen)
		return (wrd){5, 0};

	uint chk = 0;
	if(list || hLen)
	{	for(uint i = 0; i < lLen; i++)
			chk += hLen[i];
	
		if(54 < chk)
			return (wrd){5, 1};
	}

	wrd w;
	chr tmp = 55;
	chr *uList = malloc(55 - chk);
	chr *fList = malloc(54);

	if(!list || !hLen)
		tmp = rand() % 54;

	else
	{	uint n = 0;
		for(uint k = 0; k < 5; k++)
		{	for(uint i = 0; i < (k < 4 ? 13 : 2); i++)
			{	if(k < lLen)
				{	if(i < hLen[k])
						fList[13 * k + i] = wrd2chr(list[n++]);
	
					else
						fList[13 * k + i] = 55;
				} else
					fList[13 * k + i] = 55;
			}
		}
	
		n = 0;
		for(uint k = 0; k < 5; k++)
		{	for(uint i = 0; i < (k < 4 ? 13 : 2); i++)
			{	if(fList[13 * k + i] != 55)
					uList[n++] = 13 * k + i;
			}
		}

		tmp = uList[rand() % (55 - chk)];
	}

	w = chr2wrd(tmp);

	free(fList);
	free(uList);

	return w;
}

#endif /* trump.h */
