#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define	MAX_NUM_DIGITS 1000

#define PLUS	1
#define MINUS	-1

typedef struct {
	int integers[MAX_NUM_DIGITS];	// The decimal part.
	int fractions[MAX_NUM_DIGITS];	// The fraction part.
	int sign;					// 0 means minus, 1 means plus.
	int numIntegers;			// Size of integers.
	int numFractions;			// Size of fractions.
} bignum;

void initializeBignum(bignum *);
void subtractBignum(bignum *, bignum *, bignum *);
int divide_bignum(bignum *, bignum *, bignum *);
void leftShift(bignum *, int);
void rightShift(bignum *, int);
void multiply_bignum(bignum *, bignum *, bignum *);
void optimizeBignum(bignum *);
int compareBignum(bignum *, bignum *);
int isBignumZero(bignum *);
int getMax(int, int);
void analizeNumString(char *string, int *numChars, int *numIntegers, int *numFractions);

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

	for (; i < MAX_NUM_DIGITS; i++)
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

void addBignum(bignum *a, bignum *b, bignum *c)
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
			subtractBignum(b, a, c);
			a->sign = MINUS;
		}
		else
		{
			b->sign = PLUS;
			subtractBignum(a, b, c);
			b->sign = MINUS;
		}

		return;
	}

	if (a->sign == MINUS)
	{
		a->sign = b->sign = PLUS;
		addBignum(a, b, c);
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

int compareBignum(bignum *a, bignum *b)
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

void subtractBignum(bignum *a, bignum *b, bignum *c)
{
	int carry = 0;
	int i = 0;
	int maxNumFractions = getMax(a->numFractions, b->numFractions);
	int maxNumDecimals = getMax(a->numIntegers, b->numIntegers);
	int left, right;
	int result;

	// Determine if the two numbers are both positive or negative. If not, call the add function.
	if (a->sign != b->sign)
	{
		// Call substract.
		if (a->sign == MINUS)
		{
			b->sign = MINUS;
			addBignum(a, b, c);
			b->sign = PLUS;
		}
		else
		{
			b->sign = PLUS;
			addBignum(a, b, c);
			b->sign = MINUS;
		}

		return;
	}

	if (a->sign == MINUS)
	{
		b->sign = PLUS;
		addBignum(b, a, c);	
		b->sign = MINUS;
		return;
	}

	if (compareBignum(a, b) < 0)
	{
		subtractBignum(b, a, c);
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

void leftShift(bignum *num, int movement)
{
	int i, len;

	if (movement <= 0)
	{
		return;
	}

	if (isBignumZero(num))
	{
		return;
	}

	// Shift the integers.
	for (i = num->numIntegers - 1; i >= 0; i--)
	{
		num->integers[i + movement] = num->integers[i];
	}

	// Move the fractions to integers.
	for (i = 0; i < movement; i++)
	{
		num->integers[i] = num->fractions[movement - 1 - i];
	}

	// Move the fractions.
	for (i = 0, len = num->numFractions - movement; i < len; i++)
	{
		num->fractions[i] = num->fractions[i + movement];
	}

	num->numIntegers += movement;
	num->numFractions -= movement;

	if (num->numFractions < 0)
	{
		num->numFractions = 0;
	}

	optimizeBignum(num);
}

void rightShift(bignum *num, int movement)
{
	int i;

	if (movement <= 0)
	{
		return;
	}

	if (isBignumZero(num))
	{
		return;
	}

	// Move the fractions.
	for (i = num->numFractions + movement - 1; i >= movement; i--)
	{
		num->fractions[i] = num->fractions[i - movement];
	}

	// Move the integers to fractions.
	for (i = 0; i < movement; i++)
	{
		num->fractions[i] = num->integers[movement - 1 - i];
	}

	// Shift the integers.
	for (i = 0; i < num->numIntegers; i++)
	{
		num->integers[i] = num->integers[i + movement];
	}

	num->numIntegers -= movement;

	if (num->numIntegers < 1)
	{
		num->numIntegers = 1;
		num->integers[0] = 0;
	}

	num->numFractions += movement;
	optimizeBignum(num);
}

void multiply_bignum(bignum *a, bignum *b, bignum *c)
{
	bignum aCopy = *a;
	bignum tmp;
	int i, j, len;
	int digit;

	// TODO: compare a and b for the optimized performance.
	if (compareBignum(a, b) < 0)
	{
		multiply_bignum(b, a, c);
		return;
	}

	initializeBignum(c);
	rightShift(&aCopy, b->numFractions);

	// Multiple by the fractional numbers.
	for (i = 0; i < b->numFractions; i++)
	{
		digit = b->integers[i];

		// Add {digit} times.
		for (j = 1; j <= digit; j++)
		{
			addBignum(c, &aCopy, &tmp);
			*c = tmp;
		}

		leftShift(&aCopy, 1);
	}

	// Multiple by the integer numbers.
	for (i = 0; i <= b->numIntegers; i++)
	{
		digit = b->integers[i];

		for(j = 1; j <= digit; j++)
		{
			addBignum(c, &aCopy, &tmp);
			*c = tmp;
		}

		leftShift(&aCopy, 1);
	}

	c->sign = (a->sign == b->sign) ? PLUS : MINUS;

	optimizeBignum(c);
}

int divide_bignum(bignum *a, bignum *b, bignum *c)
{
	bignum row;                     /* represent shifted row */
	bignum tmp;                     /* placeholder bignum */
	int aSign, bSign;				/* temporary signs */
	int i;							/* counters */
	int numShiftRequired;
	int answerIndex;
	bignum aCopy, bCopy, tempNum;
	int compareResult;
	int count = 0;
	int isExiting = 0;
	int isIntegersDone = 0;

	if (isBignumZero(b))
	{
		// Divided by zero.
		return EXIT_FAILURE;
	}

	initializeBignum(c);

	if (isBignumZero(a))
	{
		return EXIT_SUCCESS;
	}

	c->sign = (a->sign == b->sign) ? PLUS : MINUS;

	aSign = a->sign;
	bSign = b->sign;
	a->sign = b->sign = PLUS;

	aCopy = *a;
	bCopy = *b;

	if (b->numIntegers == 1 && b->integers[0] == 0)
	{
		for (i = 0; i < MAX_NUM_DIGITS; i++)
		{
			if (b->fractions[i] != 0)
			{
				break;
			}
		}

		numShiftRequired = (a->numIntegers - b->numIntegers) + i + 1;
	}
	else
	{
		numShiftRequired = (a->numIntegers > b->numIntegers) ? a->numIntegers - b->numIntegers : 0;
	}
	
	c->numIntegers = numShiftRequired + 1;
	leftShift(&bCopy, numShiftRequired);
	answerIndex = numShiftRequired;

	// Fill up the integers.
	while (answerIndex >= 0)
	{
		while (1)
		{
			if (isBignumZero(&aCopy))
			{
				isExiting = 1;
				break;
			}

			compareResult = compareBignum(&aCopy, &bCopy);

			if (compareResult < 0)
			{
				if (--answerIndex < 0)
				{
					isIntegersDone = 1;
					break;
				}

				leftShift(&aCopy, 1);
				continue;
			}

			subtractBignum(&aCopy, &bCopy, &tempNum);
			aCopy = tempNum;
			c->integers[answerIndex]++;
		}

		if (isIntegersDone || isExiting)
		{
			break;
		}
	}

	if (!isExiting)
	{
		answerIndex = 0;		
		leftShift(&aCopy, 1);
		c->numFractions++;

		// Fill up the fractions.
		while (answerIndex < MAX_NUM_DIGITS - 1)
		{
			while (1)
			{
				if (isBignumZero(&aCopy))
				{
					isExiting = 1;
					break;
				}

				compareResult = compareBignum(&aCopy, &bCopy);

				if (compareResult < 0)
				{
					if (++answerIndex >= MAX_NUM_DIGITS - 1)
					{
						isExiting = 1;
						break;
					}

					leftShift(&aCopy, 1);
					c->numFractions++;

					continue;
				}

				subtractBignum(&aCopy, &bCopy, &tempNum);
				aCopy = tempNum;
				c->fractions[answerIndex]++;
			}

			if (isExiting)
			{
				break;
			}
		}
	}

	optimizeBignum(c);

	a->sign = aSign;
	b->sign = bSign;

	return EXIT_SUCCESS;
}

int isBignumZero(bignum *num)
{
	if ((num->numIntegers == 1) && (num->integers[0] == 0) && (num->numFractions == 0))
	{
		return 1;
	}

	return 0;
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

	optimizeBignum(num);

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

	int numsSize = 100;
	bignum *nums = (bignum *) malloc(sizeof(bignum) * numsSize);
	bignum num1, num2, num3;
	char *string = NULL;
	int answer;

	for (i = 0; i < numsSize; i++)
	{
		initializeBignum(&nums[i]);
	}

	//// Test compare
	//printf("Test comparing\n");
	//len = 0;
	//stringToBignum("12.34", &nums[len++]);

	//stringToBignum("12", &nums[len++]);
	//stringToBignum("12.35", &nums[len++]);
	//stringToBignum("12.33", &nums[len++]);
	//stringToBignum("12.3", &nums[len++]);
	//stringToBignum("12.345", &nums[len++]);
	//stringToBignum("-12.35", &nums[len++]);
	//stringToBignum("-12.33", &nums[len++]);
	//stringToBignum("-12.3", &nums[len++]);
	//stringToBignum("-12.345", &nums[len++]);
	//stringToBignum("0", &nums[len++]);
	//stringToBignum("-0", &nums[len++]);

	//for (i = 1; i < len; i++)
	//{
	//	answer = compareBignum(&nums[0], &nums[i]);
	//	string = (answer == 0) ? "=" : ((answer > 0) ? ">" : "<");
	//	printf("%s %s %s\n", bignumToString(&nums[0]), string, bignumToString(&nums[i]));
	//}

	//printf("===================\n");
	//len = 0;
	//stringToBignum("-12.34", &nums[len++]);

	//stringToBignum("12", &nums[len++]);
	//stringToBignum("12.35", &nums[len++]);
	//stringToBignum("12.33", &nums[len++]);
	//stringToBignum("12.3", &nums[len++]);
	//stringToBignum("12.345", &nums[len++]);
	//stringToBignum("-12.35", &nums[len++]);
	//stringToBignum("-12.33", &nums[len++]);
	//stringToBignum("-12.3", &nums[len++]);
	//stringToBignum("-12.345", &nums[len++]);
	//stringToBignum("0", &nums[len++]);
	//stringToBignum("-0", &nums[len++]);

	//for (i = 1; i < len; i++)
	//{
	//	answer = compareBignum(&nums[0], &nums[i]);
	//	string = (answer == 0) ? "=" : ((answer > 0) ? ">" : "<");
	//	printf("%s %s %s\n", bignumToString(&nums[0]), string, bignumToString(&nums[i]));
	//}

	//// Test math.
	//printf("\nTest addition\n");
	//len = 0;
	//stringToBignum("1.1", &nums[len++]);
	//stringToBignum("-2.2", &nums[len++]);
	//stringToBignum("0", &nums[len++]);
	//stringToBignum("-0", &nums[len++]);

	//for (i = 0; i < len; i++)
	//{
	//	for (j = 0; j < len; j++)
	//	{
	//		num1 = nums[i];
	//		num2 = nums[j];
	//		initializeBignum(&num3);
	//		addBignum(&num1, &num2, &num3);
	//		printf("%s + %s = %s\n", bignumToString(&num1), bignumToString(&num2), bignumToString(&num3));
	//	}
	//}

	//printf("\nTest subtract\n");
	//len = 0;
	//stringToBignum("1.1", &nums[len++]);
	//stringToBignum("-2.2", &nums[len++]);
	//stringToBignum("0", &nums[len++]);
	//stringToBignum("-0", &nums[len++]);

	//for (i = 0; i < len; i++)
	//{
	//	for (j = 0; j < len; j++)
	//	{
	//		num1 = nums[i];
	//		num2 = nums[j];
	//		initializeBignum(&num3);
	//		subtractBignum(&num1, &num2, &num3);
	//		printf("%s - %s = %s\n", bignumToString(&num1), bignumToString(&num2), bignumToString(&num3));
	//	}
	//}

	//printf("\nTest shift\n");
	//len = 0;
	//stringToBignum("1.00", &nums[len++]);
	//stringToBignum("0.01", &nums[len++]);
	//stringToBignum("-7654.3210", &nums[len++]);
	//stringToBignum("123.456", &nums[len++]);
	//stringToBignum("00000000.0000000000", &nums[len++]);
	//stringToBignum("1.1", &nums[len++]);

	//for (i = 0; i < len; i++)
	//{
	//	num1 = nums[i];
	//	j = i + 1;
	//	printf("%s << %d = ", bignumToString(&num1), j);
	//	leftShift(&num1, j);
	//	printf("%s\n", bignumToString(&num1));
	//}

	//printf("===============\n");
	//len = 0;
	//stringToBignum("1.00", &nums[len++]);
	//stringToBignum("0.01", &nums[len++]);
	//stringToBignum("-7654.3210", &nums[len++]);
	//stringToBignum("123.456", &nums[len++]);
	//stringToBignum("00000000.0000000000", &nums[len++]);
	//stringToBignum("1.1", &nums[len++]);

	//for (i = 0; i < len; i++)
	//{
	//	num1 = nums[i];
	//	j = i + 1;
	//	printf("%s >> %d = ", bignumToString(&num1), j);
	//	rightShift(&num1, j);
	//	printf("%s\n", bignumToString(&num1));
	//}

	//printf("\nTest multiple\n");
	//len = 0;
	//stringToBignum("2", &nums[len++]);
	//stringToBignum("-3", &nums[len++]);
	//stringToBignum("-7.7", &nums[len++]);
	//stringToBignum("11.11", &nums[len++]);
	//stringToBignum("0", &nums[len++]);

	//for (i = 0; i < len; i++)
	//{
	//	for (j = 0; j < len; j++)
	//	{
	//		num1 = nums[i];
	//		num2 = nums[j];
	//		initializeBignum(&num3);
	//		multiply_bignum(&num1, &num2, &num3);
	//		printf("%s * %s = %s\n", bignumToString(&num1), bignumToString(&num2), bignumToString(&num3));
	//	}
	//}

	printf("\nTest divide\n");
	len = 0;
	stringToBignum("99999", &nums[len++]);
	stringToBignum("9", &nums[len++]);
	stringToBignum("-7", &nums[len++]);
	stringToBignum("1.5", &nums[len++]);
	stringToBignum("-0.03", &nums[len++]);
	stringToBignum("0", &nums[len++]);

	for (i = 0; i < len; i++)
	{
		for (j = 0; j < len; j++)
		{
			if (i == j)
			{
				continue;
			}

			num1 = nums[i];
			num2 = nums[j];
			initializeBignum(&num3);
			isOkay = divide_bignum(&num1, &num2, &num3);

			if (isOkay == EXIT_FAILURE)
			{
				printf("%s / %s = %s\n", bignumToString(&num1), bignumToString(&num2), "Error: divided by zero");
			}
			else
			{
				printf("%s / %s = %s\n", bignumToString(&num1), bignumToString(&num2), bignumToString(&num3));
			}
		}
	}

	system("pause");
}
