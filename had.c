#include <stdio.h>
#include "trump.h"

int main(void)
{	rinit();
	wrd w[54];
	chr l[1];

	winit((wrd *)&w, 54);

	w[0] = suffle(NULL, NULL, 1);
	for(chr i = 1; i < 54; i++)
	{	l[0] = i;

		w[i] = suffle(w, l, 1);
		printf("%d#%d\n", getSid(w[i]), getCid(w[i]));
	}

	return 0;
}
