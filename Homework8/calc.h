#define NUMBER '0'

int getch(FILE *fp);
int getop(char s[],  FILE *fp);
void ungetch(int);
int RPNCalc( FILE *fpi, FILE *fpo);

void push(double);
double pop(void);