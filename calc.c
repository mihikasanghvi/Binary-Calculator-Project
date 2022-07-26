#include<stdio.h>
#include<stdlib.h>
#include "calc.h"
#define PLUS 1
#define MINUS -1
#include <string.h>
#include <math.h>
#include <limits.h>
#define OPERAND	100
#define OPERATOR 200
#define END	300
#define ERR 400

//initializing the num and putting the sign as PLUS
void init(num *n) {
	n->h = n->t = NULL;
	n->decimal = 0;
	n->sign = PLUS;
}
//to adding a digit in the beginning of the number.
void appendDig(num *m, int n){
	if(n < 0 || n > 9)
		return;

	node *nn = (node *)malloc(sizeof(node));
	if(!nn)
		return;
	nn->num = n;
	nn->prev = NULL;
	if((m->h == NULL) && (m->t == NULL)){
		nn->next = NULL;
		m->h = m->t = nn;
		return;
	}
	nn->next = m->h;
	m->h->prev = nn;
	m->h =  nn;
	return;
}
//this is for adding a digit at the end of the number as a char
void addDig(num *n, char ch) {
	if(ch < '0' || ch > '9')
		return;
	node *nn = (node *)malloc(sizeof(node));
	if(nn == NULL)
		return;
	nn->num = ch - '0';//converting the char passed into an int
	nn->next = NULL;
	if((n->h == NULL) && (n->t == NULL)) {
		nn->prev = NULL;
		n->h = nn;
		n->t = nn;
		return;
	}
	nn->prev = n->t;
	n->t->next = nn;
	n->t = nn;
	return;
}

//Calculating the length of the number
int length(num *n){
	node *ptr;
	ptr = n->h;
	int count = 0;
	while(ptr != NULL){
		count++;
		ptr = ptr->next;
	}
	return count;
}
//To remove the extra zeros in the beginning of the number.
//This is done because the zeros on the left at the beginning are not significant.
void removeZero(num *n) {
	node *ptr = n->h;
	int i, l = length(n);
	for(i = 0; i < l - n->decimal -1; i++){
		if(ptr->num == 0){
			n->h = ptr->next;
			ptr->next = NULL;
			n->h->prev = NULL;
			free(ptr);
			ptr = n->h;
		}else {
			break;
		}

	}
	ptr = n->t;
	int deci = n->decimal;
	for(i = 0; i < deci -1; i++){
		if(ptr->num == 0){
			n->t = ptr->prev;
			ptr->prev = NULL;
			n->t->next = NULL;
			free(ptr);
			ptr = n->t;
			n->decimal=n->decimal-1;
		}else{
			break;
		}
	}
	return;
}

//This will remove digits at a particular index/position
void removeDig(num *n, int pos){
	int i;
	node *ptr, *temp;
	if(pos < 0 || pos >= length(n))
		return;

	ptr = n->h;
	for(i=0; i < pos - 1;i++)
		ptr = ptr->next;
	if(n->h == NULL) {
		return;
	}
	if(length(n) == 1) {
		n->h = n->t = ptr = NULL;
		return;
	}
	if(pos == 0){
		ptr->next->prev = NULL;
		n->h = ptr->next;
		free(ptr);
		return;
	}
	if(pos == length(n) - 1) {
		temp = ptr->next;
		ptr->next = NULL;
		n->t = ptr;
		free(temp);
		return;
	}

	temp = ptr->next;
	ptr->next = ptr->next->next;
	temp->next->prev = ptr;
	free(temp);
	return;
}

//Traversing the number
void traverse(num *n){
	node *ptr;
	int pos = 0;
	removeZero(n);
	ptr = n->h;
	if(n->sign == MINUS){
		printf("-");
		n->sign = PLUS;
	}
	while(ptr != NULL){
		pos++;
		if(pos == (length(n) - n->decimal + 1))
			printf(".");
		printf("%d" , ptr->num);
		ptr = ptr->next;
	}
	printf("\n");
}

//This will add zeros to the number so that the number after the decimal points will be equal length
void AddZeroDec(num *n,num *m) {
    int ndec=n->decimal;
    int mdec=m->decimal;
    if(ndec>mdec) {
        int diff1=ndec-mdec;
        for(int i=0 ; i<diff1 ; i++) {

            addDig(m,'0');
            m->decimal=m->decimal+1;

        }
    }
    if(ndec<mdec) {
        int diff2=mdec-ndec;
        for(int i=0 ; i<diff2 ; i++) {

            addDig(n,'0');
            n->decimal=n->decimal+1;

        }
    }

}

//Appending zeros to beginning of smaller number to make the numbers length the same
void AppendZero(num *n,num *m) {
    int ln=length(n);
    int lm=length(m);
    if(ln>lm) {
        int dif=ln-lm;
        for(int i=0; i<dif ; i++) {
            appendDig(m,0);
        }
    }
    if(ln<lm) {
        int dif=lm-ln;
        for(int i=0; i<dif ; i++) {
            appendDig(n,0);
        }
    }

}

//Compare the numbers
int compareNums(num *n,num *m) {
    AddZeroDec(n,m);
    AppendZero(n,m);
    node *nptr,*mptr;
    int len=length(n);
    nptr=n->h;
    mptr=m->h;
    for(int i = 0; i < len; i++) {
        if(nptr->num > mptr->num ) {
            return 1;//n is greater than m
        }
        else if(nptr->num < mptr->num ){
            return -1;//n is lesser than m
        }
        nptr=nptr->next;
        mptr=mptr->next;
    }
    return 0;//same number

}

//This is to check if the given num is zero because in cases for division it will be required.
int IsZero(num *n) {
	int zero = 0;
	node *ptr = n->h;
	for(int i = 0; i < length(n); i++) {
		if(ptr->num != 0)
			zero = 1;
		ptr = ptr->next;
	}
	return zero;
}
//Addition
num *AddNumbers(num *n, num *m) {
    num *sum;
    sum = (num *)malloc(sizeof(num));
    init(sum);
    AddZeroDec(n,m);
	//If signs not equal to subtract the two numbers
    /*if(n->sign!=m->sign) {
        if(n->sign==PLUS) {
            m->sign=PLUS;
            SubNums(n,m);
        }
        else {
            n->sign=PLUS;
            SubNums(m,n);
        }
    }*/

	//when signs equal them add
    if(n->sign == m->sign){
			int no1, no2, c = 0, s;
			int ln, lm;
			node *ptr1 = n->t;
			node *ptr2 = m->t;
			ln = length(n);
			lm = length(m);
			//handling the sign of sum
			if(n->sign == MINUS)
				sum->sign = MINUS;
			else
				sum->sign = PLUS;
			if(ln >= lm) {
				//For the smaller number (m)
				for(int i = 1; i <= lm ; i++) {
					no1 = ptr1->num;
					no2 = ptr2->num;
					s = no1 + no2 + c;
					c = s / 10;
					s = s % 10;
					appendDig(sum, s);
					ptr1 = ptr1->prev;
					ptr2 = ptr2->prev;
			}
			//for the remaining part of larger number (n)
			for(int i = 1; i <= ln - lm; i++) {
					no1 = ptr1->num;
					s = no1 + c;
					c = s / 10;
					s = s % 10;
					appendDig(sum, s);
					ptr1 = ptr1->prev;
			}

		}
		else{
			//For the smaller number (n)
			for(int i = 1; i <= ln ; i++) {
					no1 = ptr1->num;
					no2 = ptr2->num;
					s = no1 + no2 + c;
					c = s / 10;
					s = s % 10;
					appendDig(sum, s);
					ptr1 = ptr1->prev;
					ptr2 = ptr2->prev;
			}
			//for remaining part of larger number (m)
			for(int i = 1; i <= lm -ln; i++){
				no1 = ptr2->num;
				s = no1 + c;
				c = s / 10;
				s = s % 10;
				appendDig(sum, s);
				ptr2 = ptr2->prev;
			}

		}
		//making decimals of sum the same as one of the two numbers(as decimals made equal in the beginning)
		sum->decimal = n->decimal;
		if(c != 0)
			appendDig(sum, c); //if there is a carry remaining at the end.
	}
	return sum;
}

//Subtraction
num *SubNumbers(num *n, num *m) {
	num *difference;
	difference = (num *)malloc(sizeof(num));
	init(difference);
	//making both lengths equal
	AddZeroDec(n, m);
	AppendZero(n, m);
	//if signs are different then adding and change sign value
	if(n->sign != m->sign) {
		if(n->sign == MINUS) {
			n->sign = PLUS;
			difference = AddNumbers(n, m);
			difference->sign = MINUS;
		}
		else if(m->sign == MINUS) {
			m->sign = PLUS;
			difference = AddNumbers(n, m);
			difference->sign = PLUS;
		}
	}
	//if signs are equal then subtraction is performed
	else if(n->sign == m->sign) {
		//
		if(n->sign == MINUS) {
			n->sign = m->sign = PLUS;
			difference =SubNumbers(m, n);
		}
		if(n->sign == PLUS) {
			int no1, no2, d, b = 0, i , l;//difference = d; borrow = b;
			node *ptr1 = n->t;
			node *ptr2 = m->t;
			//length of both the num are same
			l = length(m);
				if(compareNums(n, m) == 1) {
					for(i = 1; i <= l; i++){
						no1 = ptr1->num;
						no2 = ptr2->num;
						no1 = no1 - b;
						if(no1 >= no2) {
							d = no1 - no2;
							b = 0;
							appendDig(difference, d);
						}
						else{
							no1 = no1 + 10;
							d = no1 - no2;
							b = 1;
							appendDig(difference, d);
						}
						ptr1 = ptr1->prev;
						ptr2 = ptr2->prev;
				}
		}
		else if(compareNums(n, m) == -1) {
			//changing difference sign based on the bigger number
			difference->sign = MINUS;
			for(i = 1; i <= l; i++){
						no1 = ptr1->num;
						no2 = ptr2->num;
						no2 = no2 - b;
						if(no2 >= no1) {
							d = no2 - no1;
							b = 0;
							appendDig(difference, d);
						}
						else{
							no2 = no2 + 10;
							d = no2 - no1;
							b = 1;
							appendDig(difference, d);
						}
						ptr1 = ptr1->prev;
						ptr2 = ptr2->prev;
				}
			}
			//if both numbers are the same append zero to the difference
			else{
				if(compareNums(n, m) == 0){
					appendDig(difference, 0);
				}
			}
		}
	}
	//
	difference->decimal = n->decimal;
	return difference;
}

//Multiplication
num *MultNumbers(num *n, num *m) {
	num *product = (num *)malloc(sizeof(num));
	init(product);
	//If both the signs are same then product->sign is PLUS
	if(n->sign == m->sign) {
		product->sign = PLUS;
		n->sign = m->sign = PLUS;
	}
	//else negative
	else{
		product->sign = MINUS;
		n->sign = m->sign = PLUS;
	}
	int ln = length(n);
    int lm = length(m);
	if(ln<lm) {
		product = MultNumbers(m, n);
		return product;
	}
	else{
		node *p1, *p2;
		int i, j, no1 = 0, no2 = 0;
		int tempres[2*ln]; //tempres is for storing temporary result
		for(i = 0; i < 2 * ln ; i++)
			tempres[i] = 0;
		int k = 2 * ln - 1;//2*ln because max length of result can be 2 times it(10^3*10^3=10^6)
		p2 = m->t;
		for(i = 0; i < lm; i++) {
			p1 = n->t;
			int c1 = 0, c2 = 0; // c1 = carry1 & c2 = carry2
			for(j = k - i; j > ln - 2; j--) {
				if(p1 != NULL && p2 != NULL) {
					no1 = p1->num *  p2->num + c1;
					p1 = p1->prev;
					c1 = no1 / 10;
					no1 = no1 % 10;
					no2 = tempres[j] + no1 + c2;
					c2 = no2 / 10;
					no2 = no2 % 10;
					tempres[j] = no2;
				}
				else{
					break;
				}
			}
			tempres[j] = c1 + c2 + tempres[j];
			ln--;
			p2 = p2->prev;
		}
		for(i = k; i >= ln - 1 && i >= 0; i--) {
			appendDig(product, tempres[i]);
		}
		product->decimal = n->decimal + m->decimal;//(0.01*0.01=0.0001(4 decimal places))
		return product;
	}
}
//Division
num *DivNumbers(num *n, num *m) {
	if(IsZero(m) == 0) {
		printf("INVALID INPUT: Mathematical Error");
		return NULL;
	}

    removeZero(n);
	removeZero(m);
	num *quo;//quotient
	quo= (num *)malloc(sizeof(num));
	num*temp;//temporary storage usage
	temp = (num *)malloc(sizeof(num));
	init(temp);
	init(quo);
	int l1, sign1, sign2;
	sign1 = n->sign;
	sign2 = m->sign;
	//sign handled
	if(n->sign == m->sign) {
		quo->sign = PLUS;
	    n->sign = m->sign = PLUS;
	}
	else {
		quo->sign = MINUS;
		n->sign = m->sign = MINUS;
	}

	int a, b;
	int i, j, k, num;
	l1 = n->decimal - m->decimal;
	a = n->decimal;
	b = m->decimal;
	n->decimal = m->decimal = 0;
	node *p;
	p = n->h;
	for(i = 0; i < length(n); i++) {
		num = p->num;
		p=p->next;
		addDig(temp, num+'0');
		if(compareNums(temp, m) < 0) {
			addDig(quo, 0+'0');
		}
		else {
			int d = 0;
			while(1) {
				temp = SubNumbers(temp, m);
				d++;
				if(compareNums(temp, m) < 0)
					break;

			}
			addDig(quo, d+'0');
		}
	}
	j = 0; k = 0;
	while(j < 10) {
		appendDig(temp, 0);
		int d = 0;
		while(compareNums(temp, m) >= 0) {
			temp = SubNumbers(temp, m);
			d++;
		}
		addDig(quo, d+'0');
		k++;j++;
	}
	quo->decimal = 10 + l1;
	n->decimal = a;
	m->decimal = b;
	n->sign = sign1;
	m->sign = sign2;
	return quo;
}
//Mod
num *ModNumbers(num *n, num *m) {
	if(IsZero(m) == 0) {
		printf("ERROR:mathematical error in expression\n");
		return NULL;
	}
	int tempsign;
	//handling signs
	if(n->sign == MINUS) {
		tempsign = MINUS;
		n->sign = m->sign = PLUS;
	}
	else{
		tempsign = PLUS;
		n->sign = m->sign = PLUS;
	}
	AddZeroDec(n, m);
	int ndec = n->decimal;
	num *mod = (num *)malloc(sizeof(num));
	num *temp = (num *)malloc(sizeof(num));
	init(mod);
	init(temp);
	temp = DivNumbers(n, m);
	if(temp->decimal != 0){
		int position = length(temp)-1;
		while(temp->decimal != 0) {
			removeDig(temp, position);
			temp->decimal=temp->decimal-1;
			position=position-1;
		}
	}
	temp = MultNumbers(temp, m);
	mod = SubNumbers(n, temp);
	mod->sign = tempsign;
	mod->decimal = ndec;
	return mod;
}
//Calculating factorial. Input should be like 5!1
num *FactNumbers(num *n,num *m) {
	num *fact = (num *)malloc(sizeof(num));
	init(fact);
	appendDig(fact, 1);
	while(compareNums(n, m) > 0) {
		fact = MultNumbers(n, fact);
		n = SubNumbers(n, m);
	}
	return fact;

}

//Calculating the Power
num *PowerNumbers(num *n,num *m){
    num *pow = (num *)malloc(sizeof(num));
    num *temp = (num *)malloc(sizeof(num));
    init(pow);
    init(temp);
    appendDig(pow,1);
    appendDig(temp,1);
    int i,j,k,num1=0;
    node *ptr = m->h;
    while(ptr!=NULL){
        int i=0;
        k = ptr->num;
        num1 = num1*(10^i) + k;
        i++;
        ptr = ptr->next;
    }
    if (n->sign!=m->sign){
        if(n->sign == MINUS){
            pow->sign = MINUS;
            n->sign = PLUS;
            PowerNumbers(n,m);
        }
        else{
            m->sign = PLUS;
            for(i=0;i<num1;i++){
                pow = MultNumbers(n,pow);
            }
            pow = DivNumbers(temp,pow);
            return pow;
        }
    }
    else{
        if(n->sign == MINUS){
            pow->sign = MINUS;
            n->sign = PLUS;
            PowerNumbers(n,m);
        }
        else{
            for(i=0;i<num1;i++){
                pow = MultNumbers(n,pow);
            }
        return pow;
        }
    }

}

//This is the code for implementing a stack that is used to store all operands
void numinit(numStack *s) {
	*s = NULL;
}


void numpush(numStack *s, num *t){
	numStack temp;
	temp = (numnode *)malloc(sizeof(numnode));
	temp->a = t;
	temp->p = (*s);
	*s = temp;
}

num *numpop(numStack *s){
	num *t;
	numStack temp;
	t = (*s)->a;
	temp = (*s);
	*s = (*s)->p;
	free(temp);
	return t;
}

int numEmp(numStack *s){
	return *s == NULL;
}

void charinit(charStack *s) {
	*s = NULL;
}


void charpush(charStack *s, char t){
	charStack temp;
	temp = (charnode *)malloc(sizeof(charnode));
	temp->a = t;
	temp->p = (*s);
	*s = temp;
}

char charpop(charStack *s){
	char t;
	charStack temp;
	temp = (*s);
	t = (*s)->a;
	*s = (*s)->p;
	free(temp);
	return t;
}

int charEmp(charStack *s){
	return *s == NULL;
}
//using finite state machine example...tokenization
// enumerate function is used
enum state { NUMBER, DOT, OP, FIN, ERROR, SPACE };
token gettoken(char *expr, int *reset) {
	static int i = 0;
	num *n = (num *)malloc(sizeof(num));
	init(n);
	char current_ch;
	static enum state current_state = SPACE;
	enum state nextstate;
	token t;
	if(*reset == 1) {
		current_state = SPACE;
		*reset = 0;
		i = 0;

	}
	while(1) {
		current_ch = expr[i];
		switch(current_state) {
			case NUMBER:
				switch(current_ch) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						nextstate = NUMBER;
						addDig(n, current_ch);
						i++;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(' : case ')' :
					case '!': case '^':
						nextstate = OP;
						t.type = OPERAND;
						t.no = n;
						current_state = nextstate;
						i++;
						return t;
						break;
					case '\0':
						nextstate = FIN;
						t.type = OPERAND;
						t.no = n;
						current_state = nextstate;
						return t;
						break;
					case ' ':
						nextstate = SPACE;
						t.type = OPERAND;
						t.no = n;
						current_state = nextstate;
						i++;
						return t;
						break;

					case '.':
						nextstate = DOT;
						i++;
						current_state = nextstate;
						break;

					default:
						nextstate = ERROR;
						t.type = OPERAND;
						t.no = n;
						current_state = nextstate;
						return t;
						break;
				}
				break;



			case OP:
				switch(current_ch) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						t.type = OPERATOR;
						t.op = expr[i - 1];
						nextstate = NUMBER;
						current_state = nextstate;
						return t;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(' : case ')' :
					case '!': case '^':
						nextstate = OP;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						current_state = nextstate;
						i++;
						return t;
						break;
					case '\0':
						nextstate = FIN;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						current_state = nextstate;
						return t;
						break;
					case ' ':
						nextstate = SPACE;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						current_state = nextstate;
						i++;
						return t;
						break;

					case '.':
						nextstate = DOT;
						t.type = OPERATOR;
						t.op = expr[i -1];
						current_state = nextstate;
						i++;
						return t;
						break;

					default:
						nextstate = ERROR;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						current_state = nextstate;
						return t;
						break;
				}
				break;
			case DOT:
				switch(current_ch) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						addDig(n, current_ch);
						n->decimal++;
						nextstate = DOT;
						current_state = nextstate;
						i++;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(' : case ')' :
					case '!': case '^':
						nextstate = OP;
						t.type = OPERAND;
						t.no = n;
						current_state = nextstate;
						i++;
						return t;
						break;
					case '\0':
						nextstate = FIN;
						t.type = OPERAND;
						t.no = n;
						current_state = nextstate;
						return t;
						break;
					case ' ':
						nextstate = SPACE;
						t.type = OPERAND;
						t.no = n;
						current_state = nextstate;
						i++;
						return t;
						break;

					case '.':
						nextstate = DOT;
						i++;
						current_state = nextstate;
						break;

					default:
						nextstate = ERROR;
						t.type = OPERAND;
						t.no = n;
						current_state = nextstate;
						return t;
						break;
				}
				break;
			case FIN:
				t.type = END;
				return t;
				break;
			case ERROR:
				t.type = ERR;
				return t;
				break;
			case SPACE:
				switch(current_ch) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':

						addDig(n, current_ch);
						nextstate = NUMBER;
						i++;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(' : case ')' :
					case '!': case '^':
						nextstate = OP;
						i++;
						break;
					case '\0':
						nextstate = FIN;
						break;
					case ' ':
						nextstate = SPACE;
						i++;
						break;

					case '.':
						nextstate = DOT;
						i++;
						break;

					default:
						nextstate = ERROR;
						break;
				}
				current_state = nextstate;
				break;
		}
	}
}
char ctop(charStack *s) {
	char ch;
	ch = charpop(s);
	charpush(s, ch);
	return ch;
}

//setting precedence of each operator that is included in BC
int precedance(char o) {
	switch(o) {
		case ')' :
			return 0;
			break;
		case '+' :
		case '-' :
			return 1;
			break;
		case '*' :
		case '/' :
			return 2;
			break;
		case '%' :
			return 3;
			break;
		case '!' :
			return 4;
			break;
        case '^':
            return 5;
            break;
		case '(' :
			return 6;
			break;
		default :
			return 7;
			break;
	}
	return 5;
}

//Function used to solve the infix expression as mentioned in beginning
num *infix(char *exp) {
	token t;
	char currop, prevop, ch;
	int cntob = 0, cntcb = 0, reset  = 1, curr, pre;
	num *n, *m, *res;
	n = (num *)malloc(sizeof(num));
	m = (num *)malloc(sizeof(num));
	res = (num *)malloc(sizeof(num));
	init(n);
	init(m);
	init(res);
	numStack is;
	charStack cs;
	numinit(&is);
	charinit(&cs);
	pre = ERR;
	while(1) {
		t = gettoken(exp, &reset);
		curr = t.type;
		if(curr == pre && pre == OPERAND) {

			return NULL;
		}
		if(t.type == OPERAND){
			numpush(&is, t.no);

		}
		else if(t.type == OPERATOR){
			currop = t.op;
			if(currop == '(')
				cntob++;
			if(currop == ')')
				cntcb++;
			if(cntcb > cntob)

				return NULL;
			if(!charEmp(&cs)) {
				prevop = ctop(&cs);
				while(precedance(prevop) >= precedance(currop)) {
					prevop = charpop(&cs);
					if(!numEmp(&is)) {
						n = numpop(&is);
					}
					else{
						charpush(&cs, prevop);
						break;
					}
					if(!numEmp(&is)) {
						m = numpop(&is);
					}
					else {
						charpush(&cs, prevop);
						numpush(&is, n);
						break;
					}
					if((prevop == '/' || prevop == '%') && n == 0) {
                                                //fprintf(stderr, "ERROR:Please recheck your expression\n");
                                                printf("ERROR:Please recheck your expression\n");

                                        	return NULL;
                                        }
					switch (prevop) {
						case '+' :

							res = AddNumbers(n, m);
							numpush(&is, res);
							break;
						case '-' :

							res = SubNumbers(m, n);
							numpush(&is, res);
							break;
						case '*' :

							res = MultNumbers(m, n);
							numpush(&is, res);
							break;
						case '/' :

							res = DivNumbers(m, n);
							numpush(&is, res);
							break;
						case '%' :

							res = ModNumbers(m, n);
							numpush(&is, res);
							break;
						case '!' :

							res = FactNumbers(m, n);
							numpush(&is, res);
							break;
                        case '^':
                            res = PowerNumbers(m,n);
                            numpush(&is,res);
                            break;
						case '(' :
							charpush(&cs, prevop);
							numpush(&is, m);
							numpush(&is, n);
							break;
						default :

							return NULL;
					}
					if (prevop == '(')
						break;
					if(!charEmp(&cs))
						prevop = ctop(&cs);
					else
						break;
				}
			}
			charpush(&cs, t.op);
			if(currop == ')') {
				ch = charpop(&cs);
				cntcb--;
				ch = charpop(&cs);
				cntob--;
			}
		}
		else if(t.type == END) {
			if(cntob == cntcb) {
				while(!charEmp(&cs)) {
					if(!numEmp(&is)) {
						n = numpop(&is);
					}
					else {
						printf("Error less number of operands\n");

                        return NULL;
					}
					if(!numEmp(&is)) {
						m = numpop(&is);
					}
					else {

						printf("Less operands\n");

                        return NULL;
					}
					ch = charpop(&cs);
					if((ch == '/' || ch == '%') && n == 0) {
                        printf("RECHECK EXPRESSION:there is a mathematical error in expression\n");
						return NULL;
					}
					switch(ch) {
						case '+' :
							res = AddNumbers(m, n);
							numpush(&is, res);
							break;
						case '-' :
							res = SubNumbers(m, n);
							numpush(&is, res);
							break;
						case '*' :
							res = MultNumbers(m, n);
							numpush(&is, res);
							break;
						case '/' :
							res = DivNumbers(m, n);
							numpush(&is, res);
							break;
						case '%' :
							res = ModNumbers(m, n);
							numpush(&is, res);
							break;
						case '!' :
							res = FactNumbers(m, n);
							numpush(&is, res);
							break;
                        case '^':
                            res = PowerNumbers(m,n);
                            numpush(&is,res);
                            break;
						default :

							return NULL;
					}
				}
			}
			else {
                printf("ERROR: There is an error in the expression\n");
				return NULL;
			}
			if(!numEmp(&is)){
				res = numpop(&is);
				if(numEmp(&is))
					return res;
				else {

                        printf("ERROR:Less number of Operators\n");
                        return NULL;
                    }
			}
			else {

				printf("ERROR:Less number of Operands\n");
				return NULL;
			}
		}
		else if(t.type == ERR) {
			return NULL;
		}
		pre = curr;
	}
}

//Readline reads the string that has been given as input and stores it in an array
int readline(char *line, int l) {
	int i;
	char ch;
	i = 0;
	while(i < l - 1) {
		ch = getchar();
		if(ch == '\n') {
			line[i++] = '\0';
			return i - 1;
		}
		else
			line[i++] = ch;
	}
	line[l - 1] = '\0';
	return l - 1;
}

