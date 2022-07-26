#include<stdio.h>
#include "calc.h"
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

int main(){

    printf("bc 1.07.1\nMihika's Binary Calculator.\n\n");
    printf("This is my attempt of creating a bc-infinite precision calculator.\nThe operations that can be done are: +, -, *, /,^,!.\n\n");
    num *r;
	char line[2048];
	while(readline(line, 2048)) {
		r = infix(line);
		if(r != NULL)
			traverse(r);
		else
			printf("ERROR: error in expression\n");
	}
	return 0;

}
