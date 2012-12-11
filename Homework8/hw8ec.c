/*
* Student name: Hsien Fang Liu , Student ID:007470203 , Date: 11/12/12 , 
* Homework 8 Assignment Polish calculator. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "calc.h"
#include "bignum.h"

#define MAXOP 100000
#define NUMBER '0'

char out[256];  //this should be global (outside RPNCalc)
char *output = out;  //this can be inside RPNCalc or outside

void printAllInput(FILE *inputFile) {
	char string[100];

	printf("\n=======================\nPrint all input:\n");

	while (fgets(string, 100, inputFile) != NULL)
	{
		printf(string);
	}

	printf("End of printing all input\n==========================\n");

	rewind(inputFile);
}

int RPNCalc( FILE *fpi, FILE *fpo)
{
	int type = 0, i = 0;
	char c = '\0';

	bignum op2 = getZeroBignum();
	char s[MAXOP];
	bignum x, y, z;

	bignum storedValues[10];
	int index = 0;

	//printAllInput(fpi);

	for (i = 0; i < 10; i++)
	{
		storedValues[i] = getZeroBignum();
	}

	while((type = getop(s, fpi)) != EOF)
	{
		switch(type)
		{
			//case -109:
			//	x = sqrt(pop());
			//	push(x);
			//	break;

		case NUMBER:
			stringToBignum(s, &x);
			push(x);
			break;

		case '+':
			x = pop();
			y = pop();
			addBignum(&x, &y, &z);
			push(z);
			break; 

		case '-':
			x = pop();
			y = pop();
			subtractBignum(&y, &x, &z);
			push(z);
			break;

		case '*':
			x = pop(); 
			y = pop();
			multiply_bignum(&x, &y , &z);
			push(z);
			break;

		case '/':
			x = pop();
			y = pop();

			if (divide_bignum(&y, &x, &z))
			{
				push(z);
			}
			else
			{
				printf("error: zero divisor\n");
				fprintf(fpo, "error: zero divisor\n");
			}

			break; 

		case '\n':
			//	x = pop();
			//	printf("\t%s\n", bignumToString(&x));
			//	fprintf(fpo, "\t%s\n", bignumToString(&x));
			//	push(x);
			break;

		case '=':
			x = pop();

			if (!isBignumZero(&x))
			{
				printf("\t%s\n", bignumToString(&x));
				fprintf(fpo, "\t%s\n", bignumToString(&x));
				push(op2);
			}

			break;

		case 'X':
			x = pop();

			if (isBignumZero(&x))
			{
				break;
			}

			y = pop();

			if (isBignumZero(&y))
			{
				break;
			}

			push(x);
			push(y);

			break;

		case 'S':	// Store the popped value to temporary storage.
			x = pop();

			if (isBignumZero(&x))
			{
				break;
			}

			getop(s, fpi);			
			index = atoi(s);
			storedValues[index] = x;

			break;

		case 'R':	// Recall the temporary storage and push into the stack.
			getop(s, fpi);			
			index = atoi(s);
			x = storedValues[index];
			push(x);

			break;

		case '#':	// The comments.
			fputc('#', fpo);

			while (1)
			{
				c = fgetc(fpi);

				printf("%c", c);
				fputc(c, fpo);

				if (c == '\n')
				{
					break;
				}

				if (c == EOF)
				{
					ungetch(c);
					break;
				}
			}

			break;

		case 'D':	// Duplicate the top item.
			x = pop();
			push(x);
			push(x);
			break;

		default:
			printf("error: unknown command %s\n", s );
			fprintf(fpo, "error: unknown command %s\n", s );	
			break;
		}
	}

	x = pop();

	if (isBignumZero(&x))
	{
		printf("\t%s\n", bignumToString(&x));
		fprintf(fpo, "\t%s\n", bignumToString(&x));
		push(x);
	}

	return 0;
}

int getop(char s[],  FILE *fp)
{
	int i = 0;
	char c = '\0';

	// Skip all spaces and tabs.
	while ((s[i] = c = getch(fp)) == ' ' || c == '\t')
		;

	if (islower(c))
	{
		// Read the following characters.
		while (1)
		{
			c = getch(fp);

			if (!islower(c))
			{
				break;
			}

			s[++i] = c;
		}

		s[++i] = '\0';

		if (c != EOF)
			ungetch(c);

		if(strcmp("sqrt", s) == 0)
			return -109;
	}

	if (!isdigit(c) && c != '.')
	{
		s[++i] = '\0';
		return c;
	}

	if (isdigit(c)) /* collect integer part */
	{
		while (isdigit(s[++i] = c = getch(fp)))
			;

		if (c != EOF && c != '.')
			ungetch(c);
	}

	if (c == '.') /* collect fraction part */
		while (isdigit(s[++i] = c = getch(fp)));

	s[i] = '\0';

	if (c != EOF)
		ungetch(c);

	return NUMBER;
}