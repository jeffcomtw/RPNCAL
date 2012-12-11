#define NUMBER '0'
#include "bignum.h"

int getch(FILE *fp);
int getop(char s[],  FILE *fp);
void ungetch(int);
int RPNCalc( FILE *fpi, FILE *fpo);

void push(bignum);
bignum pop(void);