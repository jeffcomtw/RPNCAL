/*
* Student name: Hsien Fang Liu , Student ID:007470203 , Date: 11/12/12 , 
* Homework 8 Assignment Polish calculator. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include "calc.h"

#define MAXOP 100
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

int RPNCalc( FILE *fpi, FILE *fpo) // char *RPNCalc
{
	int type = 0, i = 0;
	char c = '\0';

	double op2 = 0.0;
	char s[MAXOP];
	double x = 0, y = 0;

	double storedValues[10];
	int index = 0;

	printAllInput(fpi);

	for (i = 0; i < 10; i++)
	{
		storedValues[i] = 0.0;
	}

	while((type = getop(s, fpi)) != EOF)
	{
		switch(type)
		{
		case -100:
			x = sin(pop());
			push(x);
			break;

		case -101:
			x = cos(pop());
			push(x);
			break;
			//tan	
		case -102:
			x = tan(pop());
			push(x);
			break;
			//asin
		case -103:
			x = asin(pop());
			push(x);
			break;
			//acos
		case -104:
			x = acos(pop());
			push(x);
			break;
			//atan
		case -105:
			x = atan(pop());
			push(x);
			break;
			//exp
		case -106:
			x = exp(pop());
			push(x);
			break;
			//log
		case -107:
			x = log(pop());
			push(x);
			break;
			//pow
		case -108:
			op2 = pop();
			push(pow(pop(), op2));
			break;	
			//sqrt 
		case -109:
			x = sqrt(pop());
			push(x);
			break;

		case NUMBER:
			x = atof(s);
			push(x);
			break;

		case '+':
			x = pop() + pop();
			push(x);
			break; 

		case '-':
			op2 = pop();
			push(pop() - op2);
			break;

		case '*':
			x = pop() * pop();
			push(x);
			break;

		case '/':
			op2 =pop();

			if(op2 != 0.0)
				push(pop()/op2);
			else
				fprintf(fpo, "error: zero divisor\n");
			break; 

		case '\n':
			x = pop();
			printf("\t%.16g\n", x);
			fprintf(fpo, "\t%.16g\n", x);
			push(x);
			break;

		case '=':
			op2 = pop();

			if (op2 != 0.0)
			{
				printf("\t%.16g\n", op2);
				fprintf(fpo, "\t%.16g\n", op2);
				push(op2);
			}

			break;

		case 'X':
			x = pop();

			if (x == 0.0)
			{
				break;
			}

			y = pop();

			if (y == 0.0)
			{
				break;
			}

			push(x);
			push(y);

			break;

		case 'S':	// Store the popped value to temporary storage.
			x = pop();

			if (x == 0.0)
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

		case '#':
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
			}

			break;

		case 'D':
			op2 = pop();
			push(op2);
			push(op2);
			break;

		default:
			fprintf(fpo, "error: unknown command %s\n", s );	
			break;
		}
	}

	x = pop();
	printf("\t%.16g\n", x);
	fprintf(fpo, "\t%.16g\n", x);
	push(x);

	return 0;
}

int getop(char s[],  FILE *fp)
{
	int i = 0;
	char c = '\0';

	// Skip all spaces and tabs.
	while ((s[i] = c = getch(fp)) == ' ' || c == '\t' )
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

		if (strcmp("sin", s) == 0)
			return -100;
		if(strcmp("cos", s) == 0)
			return -101;
		if (strcmp("tan", s) == 0)
			return -102;
		if (strcmp("asin", s) == 0)
			return -103;
		if(strcmp("acos", s) == 0)
			return -104;
		if(strcmp("atan", s) == 0)
			return -105;
		if(strcmp("exp", s) == 0)
			return -106;
		if(strcmp("log", s) == 0)
			return -107;
		if(strcmp("pow", s) == 0)
			return -108;
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