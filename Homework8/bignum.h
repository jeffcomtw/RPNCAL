#pragma once
#define	MAX_NUM_DIGITS 2000

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
void addBignum(bignum *, bignum *, bignum *);
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
bignum getZeroBignum();

int stringToBignum(char*, bignum *);
char * bignumToString(bignum *);