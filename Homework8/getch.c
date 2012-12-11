#include <stdio.h>

#define BUFSIZE 100

char buf[BUFSIZE];
int bufp = 0 ; 

int getch(FILE *fp)
{
	return (bufp > 0) ? buf[--bufp] : fgetc(fp);
}

void ungetch(int c)
{
	if(bufp >= BUFSIZE)
		printf ("ungetch: too many characters \n");
	else 
		buf[bufp++] = c ;
}