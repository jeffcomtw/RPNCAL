#include <stdio.h>
#include "calc.h"

int main()
{
	FILE *fpi;
	FILE *fpo;

	fpi = fopen("input2.txt", "rt");
	fpo = fopen("output.txt", "wt");

	RPNCalc(fpi, fpo);

	fclose(fpi);
	fclose(fpo);

	return 0;
}