#include<stdio.h>
#include<stdlib.h>
#define PLUS 1
#define MINUS -1
#include <string.h>
#include <math.h>
#include <limits.h>
#define OPERAND	100
#define OPERATOR 200
#define END	300
#define ERR 400
typedef struct node {
	int num;
	struct node *next , *prev;
}node;

typedef struct num{
	int sign, decimal;
	node *h, *t;
}num;

typedef struct numnode{
	num *a;
	struct numnode *p;
}numnode;
typedef numnode *numStack;

typedef struct charnode{
	char a;
	struct charnode *p;
}charnode;
typedef charnode *charStack;

typedef struct token{
	int type;
	num *no;
	char op;
}token;

void init(num *n);
void appendDig(num *m, int n);
void addDig(num *n, char ch);
int length(num *n);
void removeZero(num *n);
void removeDig(num *n, int pos);
void traverse(num *n);
void AddZeroDec(num *n,num *m) ;
void AppendZero(num *n,num *m) ;
int compareNums(num *n,num *m);
int IsZero(num *n);
num *AddNumbers(num *n, num *m);
num *SubNumbers(num *n, num *m);
num *MultNumbers(num *n, num *m);
num *DivNumbers(num *n, num *m) ;
num *ModNumbers(num *n, num *m) ;
num *FactNumbers(num *n,num *m) ;
num *PowerNumbers(num *n,num *m);
void numinit(numStack *s);
void numpush(numStack *s, num *t);
num *numpop(numStack *s);
int numEmp(numStack *s);
void charinit(charStack *s);
void charpush(charStack *s, char t);
char charpop(charStack *s);
int charEmp(charStack *s);
token gettoken(char *expr, int *reset);
char ctop(charStack *s);
int precedance(char o);
num *infix(char *exp);
int readline(char *line, int l);

