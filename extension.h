#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include "neillsdl2.h"

#define MAXNUMTOKENS 100
#define MAXTOKENSIZE 20
#define ERROR(PHRASE) {fprintf(stderr, "'%s' occurred in %s, line %d\n", PHRASE, __FILE__, __LINE__ ); exit(2);}
#define R 255 
#define G 255
#define B 255
#define PI acos(-1) 
#define PI_DEG 180 
#define ARR_SIZE 100
#define X0 150 
#define Y0 100 
#define ANGLE0 0 
#define TYPE_NUM 1
#define TYPE_VAR 0
#define PAUSE_TIME 5000
#define GAP 50

typedef enum bool {false, true} bool;
typedef enum type {error, add, subs, multi, divi} type;

typedef struct prog{
  char str[MAXNUMTOKENS][MAXTOKENSIZE]; 
  int cw; 
  double num_list[ARR_SIZE];
  double var_list[ARR_SIZE];
} Prog;

typedef struct coordinate{
  double x0;
  double y0;
  double x1;
  double y1;
  double angle;
} Coordinate;

typedef struct stack_element{
  char c;
  struct stack_element* next;
} Elm;

typedef struct stack{
  Elm* top;
  int num;
} Stack;

typedef struct color{
    int Red;
    int Green;
    int Blue;
} color;

void drawing_INIT(SDL_Simplewin* sw, Coordinate* crd, Prog* l);
void stack_INIT(Stack* s);
void color_INIT(color* clr);
void PUSH(Stack* s, char* ch);
char POP(Stack* s);
void FREE(Stack* s);
void rand_color(color* clr);

void Main(Prog* p, Stack* s, SDL_Simplewin* sw, Coordinate* crd, Prog* l, color* clr);
void Instrctlst(Prog* p, Stack* s, SDL_Simplewin* sw, Coordinate* crd, Prog* l, color* clr);
void Instruction(Prog* p, Stack* s, SDL_Simplewin* sw, Coordinate* crd, Prog* l, color* clr);

void Fd(Prog* p, SDL_Simplewin* sw, Coordinate* crd, Prog* l, color* clr);
void Clr(Prog* p, SDL_Simplewin* sw, Coordinate* crd, Prog* l, color* clr);
void Lt(Prog* p, Coordinate* crd, Prog* l);
void Rt(Prog* p, Coordinate* crd, Prog* l);
void Set(Prog* p, Stack* s, Coordinate* crd, Prog* l, color* clr);
double Polish(Prog* p, Stack* s, Coordinate* crd, Prog* l, color* clr);
double Polish_cal(Stack* s, Prog* l, type op);
double get_operand(Stack* s, Prog* l);
int Do(Prog* p, Stack* s, SDL_Simplewin* sw, Coordinate* crd, Prog* l, color* clr);

int VarNum(Prog* p);
bool is_Var(Prog* p);
bool is_Num(Prog* p);
type is_Op(Prog* p);
int char_to_i(char var);
char str_to_c(Prog* p);
double str_to_f(Prog* p);
