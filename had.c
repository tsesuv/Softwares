#include <stdio.h>
#include "trump.h"

int main(void)
{	rinit();
	wrd my = chr2wrd(55);
	wrd dlr = chr2wrd(55);
	chr input = 0;
	chr res = 'l';

	my = suffle(NULL, 1);
	dlr = suffle(&my, 1);

	if(wrd2chr(my) < wrd2chr(dlr))
		res = 'h';

	printf("High or Low (h/l)? ");
	scanf("%c", &input);

	if(input == res)
	{	printf("You win\n");

		printf("Your   card: %c%c\n", getSch(my), getCch(my));
		printf("Dealer card: %c%c\n", getSch(dlr), getCch(dlr));
	} else
	{	printf("You lose\n");

		printf("Your   card: %c%c\n", getSch(my), getCch(my));
		printf("Dealer card: %c%c\n", getSch(dlr), getCch(dlr));
	}

	return 0;
}
