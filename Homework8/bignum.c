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
	int decimals[MAXDIGITS];	// The decimal part.
	int fractions[MAXDIGITS];	// The fraction part.
	int sign;					// 0 means minus, 1 means plus.
	int numDecimals;			// Size of decimals.
	int numFractions;			// Size of fractions.
} bignum;

void initialize_bignum(bignum *);
void subtract_bignum(bignum *, bignum *, bignum *);
void divide_bignum(bignum *, bignum *, bignum *);
void multiply_bignum(bignum *, bignum *, bignum *);
void zero_justify(bignum *);
int compare_bignum(bignum *, bignum *);
int getMax(int, int);
void analizeNumString(char *string, int *numChars, int *numDecimals, int *numFractions);

void print_bignum(bignum *n)
{
	int i;
	int len;

	if (n->sign == MINUS) 
	{
		printf("-");
	}

	for (i = n->numDecimals - 1; i >= 0; i--)
	{
		printf("%d", n->decimals[i]);
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

	count += num->numDecimals;

	if (num->numFractions > 0)
	{
		count += num->numFractions + 1;
	}

	string = (char *) malloc(sizeof(char) * count);


	if (num->sign == MINUS)
	{
		string[stringIndex++] = '-';
	}

	for (i = num->numDecimals - 1; i >= 0; i--)
	{
		string[stringIndex++] = num->decimals[i] + '0';
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

void initialize_bignum(bignum *num)
{
	int i = 0;

	for (; i < MAXDIGITS; i++)
	{
		num->decimals[i] = num->fractions[i] = 0;
	}

	num->sign = PLUS;
	num->numDecimals = 0;
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
	int maxNumDecimals = getMax(a->numDecimals, b->numDecimals);
	int left, right;
	int addition;

	// Determine if the two numbers are both positive. If not, call the substract function.
	if (a->sign != b->sign)
	{
		// Call substract.
		if (a->sign == MINUS)
		{
			a->sign = PLUS;
			subtract_bignum(b, a, c);
		}
		else
		{
			b->sign = PLUS;
			subtract_bignum(a, b, c);
		}

		return;
	}

	if (a->sign == MINUS)
	{
		a->sign = b->sign = PLUS;
		add_bignum(a, b, c);
		c->sign = MINUS;
		return;
	}

	initialize_bignum(c);
	c->numFractions = maxNumFractions;
	c->numDecimals = maxNumDecimals;

	// Adding the fractions.
	for (i = maxNumFractions - 1; i >= 0; i--)
	{
		left = (i <= a->numFractions - 1) ? a->fractions[i] : 0;
		right = (i <= a->numFractions - 1) ? b->fractions[i] :  0;
		addition = left + right + carry;

		if (addition >= 10)
		{
			carry = 1;
			addition -= 10;
		}
		else
		{
			carry = 0;
		}

		c->fractions[i] = addition;
	}

	// Adding the decimals.
	for (i = 0; i < maxNumDecimals; i++)
	{
		left = (i < a->numDecimals) ? a->decimals[i] : 0;
		right = (i < b->numDecimals) ? b->decimals[i] : 0;
		addition = left + right + carry;

		if (addition >= 10)
		{
			carry = 1;
			addition -= 10;
		}
		else
		{
			carry = 0;
		}

		c->decimals[i] = addition;
	}

	if (carry != 0)
	{
		c->decimals[c->numDecimals] = 1;
		c->numDecimals++;
	}

	// TODO: call optimize function.
}

int compare_bignum(bignum *a, bignum *b)
{
	int i;
	int minNumFraction;

	//zero_justify(a);
	//zero_justify(b);

	// Compaire the sign.
	if (a->sign != b->sign)
	{
		return (a->sign == PLUS) ? 1 : -1;
	}

	// Compare the decimals.
	if (a->numDecimals > b->numDecimals)
	{
		return a->sign;
	}

	if (b->numDecimals > a->numDecimals)
	{
		return b->sign;
	}

	for (i = a->numDecimals - 1; i >= 0; i--)
	{
		if (a->decimals[i] > b->decimals[i])
		{
			return a->sign;
		}

		if (b->decimals[i] > a->decimals[i])
		{
			return b->sign;
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
			return b->sign;
		}
	}

	if (a->numFractions > b->numFractions)
	{
		return a->sign;
	}

	if (b->numFractions > a->numFractions)
	{
		return b->sign;
	}

	return 0;
}

void subtract_bignum(bignum *a, bignum *b, bignum *c)
{
	int carry = 0;
	int i = 0;
	int maxNumFractions = getMax(a->numFractions, b->numFractions);
	int maxNumDecimals = getMax(a->numDecimals, b->numDecimals);
	int left, right;
	int addition;

	// Determine if the two numbers are both positive. If not, call the add function.
	if (a->sign != b->sign)
	{
		// Call substract.
		if (a->sign == MINUS)
		{
			b->sign = MINUS;
			add_bignum(a, b, c);
		}
		else
		{
			b->sign = PLUS;
			add_bignum(a, b, c);
		}

		return;
	}

	if (a->sign == MINUS)
	{
		a->sign = b->sign = PLUS;
		add_bignum(b, a, c);
		return;
	}

	initialize_bignum(c);
	c->numFractions = maxNumFractions;
	c->numDecimals = maxNumDecimals;

	// substructing the fractions.
	for (i = maxNumFractions - 1; i >= 0; i--)
	{
		left = (i <= a->numFractions - 1) ? a->fractions[i] : 0;
		right = (i <= a->numFractions - 1) ? b->fractions[i] : 0;
		addition = left - right + carry;

		if(left < right)
		{
			compare_bignum(a, b);
		}

		if (addition >= 10)
		{
			carry = 1;
			addition -= 10;
		}
		else
		{
			carry = 0;
		}

		c->fractions[i] = addition;
	}

	// substructing the decimals.
	for (i = 0; i < maxNumDecimals; i++)
	{
		left = (i < a->numDecimals) ? a->decimals[i] : 0;
		right = (i < b->numDecimals) ? b->decimals[i] : 0;
		addition = left - right + carry;
		if(left < right) 

			if (addition >= 10)
			{
				carry = 1;
				addition -= 10;
			}
			else
			{
				carry = 0;
			}

			c->decimals[i] = addition;
	}

	if (carry != 0)
	{
		c->decimals[c->numDecimals] = 1;
		c->numDecimals++;
	}

	// TODO: call optimize function.
	//int borrow;			/* has anything been borrowed? */
	//int v;				/* placeholder digit */
	//int i;				/* counter */

	//initialize_bignum(c);

	//if ((a->sign == MINUS) || (b->sign == MINUS)) {
	//	b->sign = -1 * b->sign;
	//	add_bignum(a,b,c);
	//	b->sign = -1 * b->sign;
	//	return;
	//}

	//if (compare_bignum(a,b) == PLUS) {
	//	subtract_bignum(b,a,c);
	//	c->sign = MINUS;
	//	return;
	//}

	////c->numDecimals = max(a->numDecimals,b->numDecimals);
	//borrow = 0;

	//for (i=0; i<=(c->numDecimals); i++) {
	//	v = (a->decimals[i] - borrow - b->decimals[i]);
	//	if (a->decimals[i] > 0)
	//		borrow = 0;
	//	if (v < 0) {
	//		v = v + 10;
	//		borrow = 1;
	//	}

	//	c->decimals[i] = (char) v % 10;
	//}

	zero_justify(c);
}

void zero_justify(bignum *n)
{
	while ((n->numDecimals > 0) && (n->decimals[ n->numDecimals ] == 0))
	{
		n->numDecimals --;
	}

	if ((n->numDecimals == 0) && (n->decimals[0] == 0))
		n->sign = PLUS;	/* hack to avoid -0 */
}

void digit_shift(bignum *n, int d)		/* multiply n by 10^d */
{
	int i;				/* counter */

	if ((n->numDecimals == 0) && (n->decimals[0] == 0)) return;

	for (i=n->numDecimals; i>=0; i--)
		n->decimals[i+d] = n->decimals[i];

	for (i=0; i<d; i++) n->decimals[i] = 0;

	n->numDecimals = n->numDecimals + d;
}

void multiply_bignum(bignum *a, bignum *b, bignum *c)
{
	bignum row;			/* represent shifted row */
	bignum tmp;			/* placeholder bignum */
	int i,j;			/* counters */

	initialize_bignum(c);

	row = *a;

	for (i=0; i<=b->numDecimals; i++) {
		for (j=1; j<=b->decimals[i]; j++) {
			add_bignum(c,&row,&tmp);
			*c = tmp;
		}
		digit_shift(&row,1);
	}

	c->sign = a->sign * b->sign;

	zero_justify(c);
}

void divide_bignum(bignum *a, bignum *b, bignum *c)
{
	bignum row;                     /* represent shifted row */
	bignum tmp;                     /* placeholder bignum */
	int asign, bsign;		/* temporary signs */
	int i;                        /* counters */

	initialize_bignum(c);

	c->sign = a->sign * b->sign;

	asign = a->sign;
	bsign = b->sign;

	a->sign = PLUS;
	b->sign = PLUS;

	initialize_bignum(&row);
	initialize_bignum(&tmp);

	c->numDecimals = a->numDecimals;

	for (i=a->numDecimals; i>=0; i--) {
		digit_shift(&row,1);
		row.decimals[0] = a->decimals[i];
		c->decimals[i] = 0;
		while (compare_bignum(&row,b) != PLUS) {
			c->decimals[i] ++;
			subtract_bignum(&row,b,&tmp);
			row = tmp;
		}
	}

	zero_justify(c);

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
	int isDotAppearBefore = 0;
	char c;
	int charInteger = 0;

	int decimalsIndex = 0;
	int fractionIndex = 0;

	int numChars, numDecimals, numFractions;
	analizeNumString(string, &numChars, &numDecimals, &numFractions);

	decimalsIndex = numDecimals - 1;

	for (i = 0, len = numChars; i <= len; i++) {
		c = string[i];

		if(isdigit(c))
		{
			charInteger = c - '0';

			if (isDotAppearBefore == 0)
			{
				num->decimals[decimalsIndex] = charInteger;
				decimalsIndex--;
				num->numDecimals++;
			}
			else if (isDotAppearBefore == 1)
			{
				num->fractions[k] = charInteger;
				k++;
				num->numFractions++;
			}
		}
		else if(c == '+')
		{
			num->sign = PLUS;
		}
		else if(c == '-')
		{
			num->sign = MINUS;
		}
		else if(c == '.')
		{
			isDotAppearBefore = 1;
		}
		else 
		{
			return 1;
		}
	}

	return 0;
}

void analizeNumString(char *string, int *numChars, int *numDecimals, int *numFractions) 
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
	*numDecimals = _numDecimals;
	*numFractions = _numFractions;
}

void main()
{
	int i;
	int isOkay = 0;
	char *leftNumText, *rightNumText;
	bignum leftNum, rightNum;

	int numsSize = 10;
	bignum *nums = (bignum *) malloc(sizeof(bignum) * numsSize);
	bignum num1;
	bignum num2;
	bignum num3;
	char *string = NULL;
	int answer;

	initialize_bignum(&num1);
	isOkay = stringToBignum("-12.34", &num1);
	print_bignum(&num1);

	initialize_bignum(&num2);
	isOkay = stringToBignum("-1.23", &num2);
	print_bignum(&num2);

	add_bignum(&num1, &num2, &num3);
	print_bignum(&num3);	// Expect -1.

	for (i = 0; i < 10; i++)
	{
		initialize_bignum(&nums[i]);
	}

	// Test compare
	isOkay = stringToBignum("-12.34", &nums[0]);
	isOkay = stringToBignum("12.34", &nums[1]);
	isOkay = stringToBignum("-12.34", &nums[2]);
	isOkay = stringToBignum("-10.34", &nums[3]);
	isOkay = stringToBignum("12", &nums[4]);
	isOkay = stringToBignum("-12.3", &nums[5]);

	for (i = 1; i < 6; i++)
	{
		string = bignumToString(&nums[0]);
		string = bignumToString(&nums[i]);
		answer = compare_bignum(&nums[0], &nums[i]);
		string = (answer == 0) ? "equal to" : ((answer > 0) ? "bigger than" : "smaller than");
		printf("%s is %s %s\n", bignumToString(&nums[0]), string, bignumToString(&nums[i]));
	}

	printf("Hello world");

	//while (scanf_s("%s %s\n",&leftNumText, &rightNumText) != EOF) {
	//	isOkay = stringToBignum(leftNumText, &leftNum);
	//	isOkay = stringToBignum(rightNumText, &rightNum);

	//	printf("left = %s, right = %s", bignumToString(&leftNum), bignumToString(&rightNum));

	//	/*
	//	add_bignum(&n1,&n2,&n3);
	//	printf("addition -- ");
	//	print_bignum(&n3);

	//	printf("compare_bignum a ? b = %d\n",compare_bignum(&n1, &n2));

	//	subtract_bignum(&n1,&n2,&n3);
	//	printf("subtraction -- ");
	//	print_bignum(&n3);

	//	multiply_bignum(&n1,&n2,&n3);
	//	printf("multiplication -- ");
	//	print_bignum(&n3);

	//	int_to_bignum(0,&zero);
	//	if (compare_bignum(&zero, &n2) == 0)
	//	printf("division -- NaN \n");
	//	else {
	//	divide_bignum(&n1,&n2,&n3);
	//	printf("division -- ");
	//	print_bignum(&n3);
	//	}
	//	printf("--------------------------\n");
	//	*/
	//}
}
