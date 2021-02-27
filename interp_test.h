#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#define PROGNAME "test2.txt"
#define ARR_SIZE 100

typedef enum bool {false, true} bool;
typedef enum type {error, add, subs, multi, divi} type;

typedef struct prog{
  char str[100][7]; 
  int cw; 
  double num_list[100];
  double var_list[100];
} Prog;

typedef struct coordinate{
  double x0;
  double y0;
  double x1;
  double y1;
  double angle;
} Coordinate;

typedef struct node{
  char c;
  struct node* next;
} Node;

typedef struct stack{
  Node* top;
  int num;
} Stack;

void stack_INIT(Stack* s);
void PUSH(Stack* s, char* ch);
char POP(Stack* s);
void FREE(Stack* s);

int char_to_i(char var);
char str_to_c(Prog* p);
double str_to_f(Prog* p);
int test_get(double f_num, char ch);
double test_cal(Prog* p);
