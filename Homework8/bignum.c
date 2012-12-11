/*	bignum.c
Implementation of large integer arithmetic: addition, subtraction,
multiplication, and division.

begun: February 7, 2002
by: Steven Skiena
*/

/*
Copyright 2003 by Steven S. Skiena; all rights reserved. 

Permission is granted for use in non-commerical applications
provided this copyright notice remains intact and unchanged.

This program appears in my book:

"Programming Challenges: The Programming Contest Training Manual"
by Steven Skiena and Miguel Revilla, Springer-Verlag, New York 2003.

See our website www.programming-challenges.com for additional information.

This book can be ordered from Amazon.com at

http://www.amazon.com/exec/obidos/ASIN/0387001638/thealgorithmrepo/

*/


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
//#include <math.h>

//#ifdef max
//#undef max
//#endif

#define	MAXDIGITS	10000

#define PLUS		1
#define MINUS		-1

typedef struct {
	int integers[MAXDIGITS];	// The decimal part.
	int fractions[MAXDIGITS];	// The fraction part.
	int sign;					// 0 means minus, 1 means plus.
	int numIntegers;			// Size of integers.
	int numFractions;			// Size of fractions.
} bignum;

void initializeBignum(bignum *);
void subtract_bignum(bignum *, bignum *, bignum *);
void divide_bignum(bignum *, bignum *, bignum *);
void multiply_bignum(bignum *, bignum *, bignum *);
void optimizeBignum(bignum *);
int compare_bignum(bignum *, bignum *);
int getMax(int, int);
void analizeNumString(char *string, int *numChars, int *numIntegers, int *numFractions);

void print_bignum(bignum *n)
{
	int i;
	int len;

	if (n->sign == MINUS) 
	{
		printf("-");
	}

	for (i = n->numIntegers - 1; i >= 0; i--)
	{
		printf("%d", n->integers[i]);
	}

	if (n->numFractions > 0)
	{
		printf(".");

		for (i = 0, len = n->numFractions; i<=len-1; i++)
			printf("%d", n->fractions[i]);
	}

	printf("\n");
}

char * bignumToString(bignum *num)
{
	int i;
	int len;
	int count = 1;
	char *string;
	int stringIndex = 0;

	if (num->sign == MINUS)
	{
		count++;
	}

	count += num->numIntegers;

	if (num->numFractions > 0)
	{
		count += num->numFractions + 1;
	}

	string = (char *) malloc(sizeof(char) * count);


	if (num->sign == MINUS)
	{
		string[stringIndex++] = '-';
	}

	for (i = num->numIntegers - 1; i >= 0; i--)
	{
		string[stringIndex++] = num->integers[i] + '0';
	}

	if (num->numFractions > 0)
	{
		string[stringIndex++] = '.';

		for (i = 0, len = num->numFractions; i < len; i++)
		{
			string[stringIndex++] = num->fractions[i] + '0';
		}
	}

	string[stringIndex++] = '\0';

	return string;
}

void initializeBignum(bignum *num)
{
	int i = 0;

	for (; i < MAXDIGITS; i++)
	{
		num->integers[i] = num->fractions[i] = 0;
	}

	num->sign = PLUS;
	num->numIntegers = 1;
	num->numFractions = 0;
}

int getMax(int x, int y) {
	return (x > y) ? x : y;	// If x > y, then return x, otherwise y.
}

void add_bignum(bignum *a, bignum *b, bignum *c)
{
	int carry = 0;
	int i = 0;
	int maxNumFractions = getMax(a->numFractions, b->numFractions);
	int maxNumDecimals = getMax(a->numIntegers, b->numIntegers);
	int left, right;
	int result;

	// Determine if the two numbers are both positive. If not, call the substract function.
	if (a->sign != b->sign)
	{
		// Call substract.
		if (a->sign == MINUS)
		{
			a->sign = PLUS;
			subtract_bignum(b, a, c);
			a->sign = MINUS;
		}
		else
		{
			b->sign = PLUS;
			subtract_bignum(a, b, c);
			b->sign = MINUS;
		}

		return;
	}

	if (a->sign == MINUS)
	{
		a->sign = b->sign = PLUS;
		add_bignum(a, b, c);
		a->sign = b->sign = MINUS;
		c->sign = MINUS;
		return;
	}

	initializeBignum(c);
	c->numFractions = maxNumFractions;
	c->numIntegers = maxNumDecimals;

	// Adding the fractions.
	for (i = maxNumFractions - 1; i >= 0; i--)
	{
		left = (i <= a->numFractions - 1) ? a->fractions[i] : 0;
		right = (i <= b->numFractions - 1) ? b->fractions[i] :  0;
		result = left + right + carry;

		if (result >= 10)
		{
			carry = 1;
			result -= 10;
		}
		else
		{
			carry = 0;
		}

		c->fractions[i] = result;
	}

	// Adding the integers.
	for (i = 0; i < maxNumDecimals; i++)
	{
		left = (i < a->numIntegers) ? a->integers[i] : 0;
		right = (i < b->numIntegers) ? b->integers[i] : 0;
		result = left + right + carry;

		if (result >= 10)
		{
			carry = 1;
			result -= 10;
		}
		else
		{
			carry = 0;
		}

		c->integers[i] = result;
	}

	if (carry != 0)
	{
		c->integers[c->numIntegers] = 1;
		c->numIntegers++;
	}

	optimizeBignum(c);
}

int compare_bignum(bignum *a, bignum *b)
{
	int i;
	int minNumFraction;

	optimizeBignum(a);
	optimizeBignum(b);

	// Compaire the sign.
	if (a->sign != b->sign)
	{
		return (a->sign == PLUS) ? 1 : -1;
	}

	// Compare the integers.
	if (a->numIntegers > b->numIntegers)
	{
		return a->sign;
	}

	if (b->numIntegers > a->numIntegers)
	{
		return -a->sign;
	}

	for (i = a->numIntegers - 1; i >= 0; i--)
	{
		if (a->integers[i] > b->integers[i])
		{
			return a->sign;
		}

		if (b->integers[i] > a->integers[i])
		{
			return -a->sign;
		}
	}

	// Compare the fractions.
	minNumFraction = (a->numFractions < b->numFractions) ? a->numFractions : b->numFractions;

	for (i = 0; i < minNumFraction; i++)
	{
		if (a->fractions[i] > b->fractions[i])
		{
			return a->sign;
		}

		if (b->fractions[i] > a->fractions[i])
		{
			return -a->sign;
		}
	}

	if (a->numFractions > b->numFractions)
	{
		return a->sign;
	}

	if (b->numFractions > a->numFractions)
	{
		return -a->sign;
	}

	return 0;
}

void subtract_bignum(bignum *a, bignum *b, bignum *c)
{
	int carry = 0;
	int i = 0;
	int maxNumFractions = getMax(a->numFractions, b->numFractions);
	int maxNumDecimals = getMax(a->numIntegers, b->numIntegers);
	int left, right;
	int result;

	// Determine if the two numbers are both positive. If not, call the add function.
	if (a->sign != b->sign)
	{
		// Call substract.
		if (a->sign == MINUS)
		{
			b->sign = MINUS;
			add_bignum(a, b, c);
			b->sign = PLUS;
		}
		else
		{
			b->sign = PLUS;
			add_bignum(a, b, c);
			b->sign = MINUS;
		}

		return;
	}

	if (a->sign == MINUS)
	{
		a->sign = b->sign = PLUS;
		add_bignum(b, a, c);
		a->sign = b->sign = MINUS;
		return;
	}

	if (compare_bignum(a, b) < 0)
	{
		subtract_bignum(b, a, c);
		c->sign *= -1;
		return;
	}

	initializeBignum(c);
	c->numFractions = maxNumFractions;
	c->numIntegers = maxNumDecimals;

	// substructing the fractions.
	for (i = maxNumFractions - 1; i >= 0; i--)
	{
		left = (i <= a->numFractions - 1) ? a->fractions[i] : 0;
		right = (i <= b->numFractions - 1) ? b->fractions[i] : 0;
		result = left - right + carry;

		if (result < 0)
		{
			carry = -1;
			result += 10;
		}
		else
		{
			carry = 0;
		}

		c->fractions[i] = result;
	}

	// substructing the integers.
	for (i = 0; i < maxNumDecimals; i++)
	{
		left = (i < a->numIntegers) ? a->integers[i] : 0;
		right = (i < b->numIntegers) ? b->integers[i] : 0;
		result = left - right + carry;

		if (result < 0)
		{
			carry = -1;
			result += 10;
		}
		else
		{
			carry = 0;
		}

		c->integers[i] = result;
	}

	optimizeBignum(c);
}

void optimizeBignum(bignum *num)
{
	while ((num->numIntegers > 1) && num->integers[num->numIntegers - 1] == 0)
	{
		if(num->integers[num->numIntegers - 1] != 0)
		{
			break;
		}

		num->numIntegers--;
	}

	while((num->numFractions > 0) && (num->fractions[num->numFractions - 1] == 0))
	{
		if(num->fractions[num->numFractions - 1] != 0)
		{
			break;
		}

		num->numFractions--;
	}

	if (num->sign == MINUS && (num->numIntegers == 1) && (num->integers[0] == 0) && (num->numFractions == 0))
	{
		num->sign = PLUS;
	}
}

void digit_shift(bignum *num, int powerOfTen)		/* multiply num by 10^powerOfTen */
{
	int i;

	if ((num->numIntegers == 1) && (num->integers[0] == 0))
	{
		return;
	}

	// Shift the integers.
	for (i = num->numIntegers - 1; i >= 0; i--)
	{
		num->integers[i + powerOfTen] = num->integers[i];
	}

	for (i=0; i<powerOfTen; i++)
	{
		num->integers[i] = 0;
	}

	num->numIntegers = num->numIntegers + powerOfTen;
}

void multiply_bignum(bignum *a, bignum *b, bignum *c)
{
	bignum row;			/* represent shifted row */
	bignum tmp;			/* placeholder bignum */
	int i,j;			/* counters */

	initializeBignum(c);

	row = *a;

	for (i=0; i<=b->numIntegers; i++)
	{
		for (j=1; j<=b->integers[i]; j++)
		{
			add_bignum(c,&row,&tmp);
			*c = tmp;
		}

		digit_shift(&row,1);
	}

	for (i=0; i<= b->numFractions; i++)
	{
		for(j=1;j<= b->fractions[i];j++)
		{
			add_bignum(c,&row,&tmp);
			*c = tmp;
		}

		digit_shift(&row,1);
	}

	if (a->sign == b ->sign)
	{
		c->sign = PLUS;
	}
	else
	{
		c->sign = MINUS;
	}

	optimizeBignum(c);
}

void divide_bignum(bignum *a, bignum *b, bignum *c)
{
	bignum row;                     /* represent shifted row */
	bignum tmp;                     /* placeholder bignum */
	int asign, bsign;		/* temporary signs */
	int i;                        /* counters */

	initializeBignum(c);

	c->sign = a->sign * b->sign;

	asign = a->sign;
	bsign = b->sign;

	a->sign = PLUS;
	b->sign = PLUS;

	initializeBignum(&row);
	initializeBignum(&tmp);

	c->numIntegers = a->numIntegers;

	for (i=a->numIntegers; i>=0; i--) {
		digit_shift(&row,1);
		row.integers[0] = a->integers[i];
		c->integers[i] = 0;
		while (compare_bignum(&row,b) != PLUS) {
			c->integers[i] ++;
			subtract_bignum(&row,b,&tmp);
			row = tmp;
		}
	}

	optimizeBignum(c);

	a->sign = asign;
	b->sign = bsign;
}

// Transfer the large number in the string to a big number.
// Return 0 if succeed, 1 if fail.
int stringToBignum(char* string, bignum *num) {
	int i;
	int	j=0;
	int k = 0;
	int len = 0;
	int hasFranction = 0;
	char c;
	int charInteger = 0;

	int decimalsIndex = 0;
	int fractionIndex = 0;

	int numChars, numIntegers, numFractions;
	analizeNumString(string, &numChars, &numIntegers, &numFractions);

	decimalsIndex = numIntegers - 1;
	initializeBignum(num);
	num->numIntegers = 0;

	for (i = 0, len = numChars; i <= len; i++) {
		c = string[i];

		if (isspace(c) || c == '\0')
		{
			break;
		}
		else if(isdigit(c))
		{
			charInteger = c - '0';

			if (hasFranction == 0)
			{
				num->integers[decimalsIndex] = charInteger;
				decimalsIndex--;
				num->numIntegers++;
			}
			else if (hasFranction == 1)
			{
				num->fractions[k] = charInteger;
				k++;
				num->numFractions++;
			}
		}
		else if(c == '-')
		{
			num->sign = MINUS;
		}
		else if(c == '.')
		{
			hasFranction = 1;
		}
		else
		{
			return EXIT_FAILURE;
		}
	}

	if (num->sign == MINUS && num->numIntegers == 1 && num->numFractions == 0)
	{
		num->sign = PLUS;
	}

	return EXIT_SUCCESS;
}

void analizeNumString(char *string, int *numChars, int *numIntegers, int *numFractions) 
{
	int _numChars = 0;
	int _numDecimals = 0;
	int _numFractions = 0;
	int i = 0;
	int isFraction = 0;
	char c = ' ';

	while((c = string[i]) != '\0')
	{
		if (c == '.')
		{
			isFraction = 1;
		}
		else if (isdigit(c))
		{
			if (isFraction)
			{
				_numFractions++;
			}
			else
			{
				_numDecimals++;
			}
		}

		_numChars++;
		i++;
	}

	*numChars = _numChars;
	*numIntegers = _numDecimals;
	*numFractions = _numFractions;
}

void main()
{
	int i, j;
	int len = 0;
	int isOkay = 0;
	char *leftNumText, *rightNumText;
	bignum leftNum, rightNum;

	int numsSize = 100;
	bignum *nums = (bignum *) malloc(sizeof(bignum) * numsSize);
	bignum num1, num2, num3;
	char *string = NULL;
	int answer;

	for (i = 0; i < numsSize; i++)
	{
		initializeBignum(&nums[i]);
	}

	// Test compare
	printf("Test comparing\n");
	len = 0;
	stringToBignum("12.34", &nums[len++]);

	stringToBignum("12", &nums[len++]);
	stringToBignum("12.35", &nums[len++]);
	stringToBignum("12.33", &nums[len++]);
	stringToBignum("12.3", &nums[len++]);
	stringToBignum("12.345", &nums[len++]);
	stringToBignum("-12.35", &nums[len++]);
	stringToBignum("-12.33", &nums[len++]);
	stringToBignum("-12.3", &nums[len++]);
	stringToBignum("-12.345", &nums[len++]);
	stringToBignum("0", &nums[len++]);
	stringToBignum("-0", &nums[len++]);

	for (i = 1; i < len; i++)
	{
		answer = compare_bignum(&nums[0], &nums[i]);
		string = (answer == 0) ? "equal to" : ((answer > 0) ? "bigger than" : "smaller than");
		printf("%s is %s %s\n", bignumToString(&nums[0]), string, bignumToString(&nums[i]));
	}

	printf("===================\n");
	len = 0;
	stringToBignum("-12.34", &nums[len++]);

	stringToBignum("12", &nums[len++]);
	stringToBignum("12.35", &nums[len++]);
	stringToBignum("12.33", &nums[len++]);
	stringToBignum("12.3", &nums[len++]);
	stringToBignum("12.345", &nums[len++]);
	stringToBignum("-12.35", &nums[len++]);
	stringToBignum("-12.33", &nums[len++]);
	stringToBignum("-12.3", &nums[len++]);
	stringToBignum("-12.345", &nums[len++]);
	stringToBignum("0", &nums[len++]);
	stringToBignum("-0", &nums[len++]);

	for (i = 1; i < len; i++)
	{
		answer = compare_bignum(&nums[0], &nums[i]);
		string = (answer == 0) ? "equal to" : ((answer > 0) ? "bigger than" : "smaller than");
		printf("%s is %s %s\n", bignumToString(&nums[0]), string, bignumToString(&nums[i]));
	}

	// Test math.
	printf("\nTest addition\n");
	len = 0;
	stringToBignum("1.1", &nums[len++]);
	stringToBignum("-2.2", &nums[len++]);
	stringToBignum("0", &nums[len++]);
	stringToBignum("-0", &nums[len++]);

	for (i = 0; i < len; i++)
	{
		for (j = 0; j < len; j++)
		{
			num1 = nums[i];
			num2 = nums[j];
			initializeBignum(&num3);
			add_bignum(&num1, &num2, &num3);
			printf("%s adds %s equal to %s\n", bignumToString(&num1), bignumToString(&num2), bignumToString(&num3));
		}
	}

	printf("\nTest multiple\n");
	len = 0;
	stringToBignum("1.1", &nums[len++]);
	stringToBignum("-2.2", &nums[len++]);
	stringToBignum("0", &nums[len++]);
	stringToBignum("-0", &nums[len++]);

	for (i = 0; i < len; i++)
	{
		for (j = 0; j < len; j++)
		{
			num1 = nums[i];
			num2 = nums[j];
			initializeBignum(&num3);
			multiply_bignum(&num1, &num2, &num3);
			printf("%s multiples %s equal to %s\n", bignumToString(&num1), bignumToString(&num2), bignumToString(&num3));
		}
	}

	system("pause");
}
