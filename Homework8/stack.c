#include <stdio.h>
#include "bignum.h"

#define MAXVAL 100 

int sp = 0; 
bignum val[MAXVAL];

void push(bignum f)
{
	if(sp < MAXVAL)
		val[sp++] = f; 
	else 
		printf ("error: stack full, cant push push %g \n", f);
}

bignum pop(void)
{
	if(sp > 0)
		return val[--sp];
	else
	{
		printf("error:stack empty \n");
		return getZeroBignum();
	}
}