#include "ccc.h"

int timel[20] =
{
	1000000, 580000, 1200000, 1260010, 1800020,
	130100, 10091, 90790, 93104, 998006,
	1200201, 1400000, 1620800, 1930071, 2300010,
	2000000, 1000000, 600000, 710400, 901002
};

int main(void)
{
	setmode(1);
	csloff(1);

	acs(0, getrow());
	printf("Progress: ");
	fflush(stdout);
	acs(10, getrow());
	int par = 0;
	int i = 0;

	setcclr(GREEN);

	while(par <= 100)
	{
		bprint("%d%%", par);

		usleep(timel[i]);

		par += 11 + (par % 2) + (par % 3);
		if((par % (par % 37)) < 18)
		{
			par--;
		}
		i = (i + 1) % 20;
	}

	resettxt(1);

	acs(0, getrow());
	bprint("              ");
	printf("Conpletion!\n");
	fflush(stdout);

	csloff(0);
	setmode(0);

	return 0;
}
