#include <stdio.h>
#include <stdlib.h>
#include "bignum.h"

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
		answer = compareBignum(&nums[0], &nums[i]);
		string = (answer == 0) ? "=" : ((answer > 0) ? ">" : "<");
		printf("%s %s %s\n", bignumToString(&nums[0]), string, bignumToString(&nums[i]));
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
		answer = compareBignum(&nums[0], &nums[i]);
		string = (answer == 0) ? "=" : ((answer > 0) ? ">" : "<");
		printf("%s %s %s\n", bignumToString(&nums[0]), string, bignumToString(&nums[i]));
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
			addBignum(&num1, &num2, &num3);
			printf("%s + %s = %s\n", bignumToString(&num1), bignumToString(&num2), bignumToString(&num3));
		}
	}

	printf("\nTest subtract\n");
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
			subtractBignum(&num1, &num2, &num3);
			printf("%s - %s = %s\n", bignumToString(&num1), bignumToString(&num2), bignumToString(&num3));
		}
	}

	printf("\nTest shift\n");
	len = 0;
	stringToBignum("1.00", &nums[len++]);
	stringToBignum("0.01", &nums[len++]);
	stringToBignum("-7654.3210", &nums[len++]);
	stringToBignum("123.456", &nums[len++]);
	stringToBignum("00000000.0000000000", &nums[len++]);
	stringToBignum("1.1", &nums[len++]);

	for (i = 0; i < len; i++)
	{
		num1 = nums[i];
		j = i + 1;
		printf("%s << %d = ", bignumToString(&num1), j);
		leftShift(&num1, j);
		printf("%s\n", bignumToString(&num1));
	}

	printf("===============\n");
	len = 0;
	stringToBignum("1.00", &nums[len++]);
	stringToBignum("0.01", &nums[len++]);
	stringToBignum("-7654.3210", &nums[len++]);
	stringToBignum("123.456", &nums[len++]);
	stringToBignum("00000000.0000000000", &nums[len++]);
	stringToBignum("1.1", &nums[len++]);

	for (i = 0; i < len; i++)
	{
		num1 = nums[i];
		j = i + 1;
		printf("%s >> %d = ", bignumToString(&num1), j);
		rightShift(&num1, j);
		printf("%s\n", bignumToString(&num1));
	}

	printf("\nTest multiple\n");
	len = 0;
	stringToBignum("2", &nums[len++]);
	stringToBignum("-3", &nums[len++]);
	stringToBignum("-7.7", &nums[len++]);
	stringToBignum("11.11", &nums[len++]);
	stringToBignum("0", &nums[len++]);

	for (i = 0; i < len; i++)
	{
		for (j = 0; j < len; j++)
		{
			num1 = nums[i];
			num2 = nums[j];
			initializeBignum(&num3);
			multiply_bignum(&num1, &num2, &num3);
			printf("%s * %s = %s\n", bignumToString(&num1), bignumToString(&num2), bignumToString(&num3));
		}
	}

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

			if (isOkay)
			{
				printf("%s / %s = %s\n", bignumToString(&num1), bignumToString(&num2), bignumToString(&num3));
			}
			else
			{
				printf("%s / %s = %s\n", bignumToString(&num1), bignumToString(&num2), "Error: divided by zero");
			}
		}
	}

	system("pause");
}
