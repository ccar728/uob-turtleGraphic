#include "parse.h"
#define PROGNAME "test2.txt"

void test_do(Prog* p);
void test_set(Prog* p);
void test_polish(Prog* p);

void Varnum(Prog* p);
int Var(Prog *p);
int Op(Prog *p);
int Num(Prog* p);

int main(void){
    int i;
    FILE* fp;
    Prog p;

    p.cw = 0;
    for(i = 0; i < MAXNUMTOKENS; i++){
        p.str[i][0] = '\0';
    }
    assert(strcmp(p.str[p.cw], "") == 0);

    if(!(fp = fopen(PROGNAME, "r"))){
        fprintf(stderr, "Can't open %s\n", PROGNAME);
        exit(2);
    }

    i = 0;
    while(fscanf(fp, "%s", p.str[i]) == 1 && i < MAXNUMTOKENS){
        i++;
    }
    fclose(fp);

    /* test struct p */
    assert(strcmp(p.str[p.cw++], "{") == 0);
    assert(strcmp(p.str[p.cw++], "DO") == 0);
    assert(strcmp(p.str[p.cw++], "A") == 0);
    assert(strcmp(p.str[p.cw++], "FROM") == 0);
    assert(strcmp(p.str[p.cw++], "1") == 0);
    assert(strcmp(p.str[p.cw++], "TO") == 0);
    assert(strcmp(p.str[p.cw++], "100") == 0);
    assert(strcmp(p.str[p.cw++], "{") == 0);
    assert(strcmp(p.str[p.cw++], "SET") == 0);
    assert(strcmp(p.str[p.cw++], "C") == 0);
    assert(strcmp(p.str[p.cw++], ":=") == 0);
    assert(strcmp(p.str[p.cw++], "A") == 0);
    assert(strcmp(p.str[p.cw++], "1.5") == 0);
    assert(strcmp(p.str[p.cw++], "*") == 0);
    assert(strcmp(p.str[p.cw++], ";") == 0);
    assert(strcmp(p.str[p.cw++], "FD") == 0);
    assert(strcmp(p.str[p.cw++], "C") == 0);
    assert(strcmp(p.str[p.cw++], "RT") == 0);
    assert(strcmp(p.str[p.cw++], "62") == 0);
    assert(strcmp(p.str[p.cw++], "}") == 0);
    assert(strcmp(p.str[p.cw], "}") == 0);

    /* test Op() */
    p.cw = 13;
    assert(strcmp(p.str[p.cw], "*") == 0);
    assert(Op(&p) == 1);

    /* test Var() */
    p.cw = 2;
    assert(strcmp(p.str[p.cw], "A") == 0);
    assert(Var(&p) == 1);

    /* test Num() */
    p.cw = 4;
    assert(strcmp(p.str[p.cw], "1") == 0);
    assert(Num(&p) == 1);

    /* test Varnum() */
    /* no need to test Fd(), Lt(), Rt() */
    p.cw = 16;
    assert(strcmp(p.str[p.cw], "C") == 0);
    Varnum(&p);
    assert(strcmp(p.str[p.cw], "RT") == 0);
    p.cw++;
    assert(strcmp(p.str[p.cw], "62") == 0);
    Varnum(&p);
    assert(strcmp(p.str[p.cw], "}") == 0);

    /* test Do() */
    p.cw = 1;
    assert(strcmp(p.str[p.cw], "DO") == 0);
    p.cw = p.cw + 2;
    test_do(&p);
    assert(strcmp(p.str[p.cw], "SET") == 0);

    /* test Set() */
    p.cw++;
    test_set(&p);
    assert(strcmp(p.str[p.cw], "A") == 0);

    /* test Polish() */
    test_polish(&p);
    assert(strcmp(p.str[p.cw], "FD") == 0);

    printf("Parser test OK!\n");
    return 0;
}

void test_do(Prog *p){
    if(strcmp(p->str[p->cw], "FROM") == 0){
        p->cw++;
    }
    Varnum(p);
    if(strcmp(p->str[p->cw], "TO") == 0){
        p->cw++;
    }
    Varnum(p);
    if(strcmp(p->str[p->cw], "{") == 0){
        p->cw++;
    }
}

void test_set(Prog* p){
    p->cw++;
    if(strcmp(p->str[p->cw], ":=") == 0){
        p->cw++;
    }
}

void test_polish(Prog* p){
    if(strcmp(p->str[p->cw], ";") == 0){
        p->cw++;
    }
    else if(Op(p)){
        test_polish(p);
    }
    else if(Var(p)){
        test_polish(p);
    }
    else if(Num(p)){
        test_polish(p);
    }
}

void Varnum(Prog* p){
    if(Num(p)){
        ;
    }
    else if(Var(p)){
        assert(1>0);
    }
}

int Op(Prog* p){
    char* op = p->str[p->cw];
    if((op[0] == '*' || op[0] == '/' || op[0] == '+' || op[0] == '-')){
        p->cw++;
        return 1;
    }
    else{
        return 0;
    }
}

int Var(Prog* p){
    char* var = p->str[p->cw];
    if(var[0] >= 'A' && var[0] <= 'Z'){
        p->cw++;
        return 1;
    }
    return 0;
}

int Num(Prog* p){
    int i = atoi(p->str[p->cw]);
    float f = atof(p->str[p->cw]);

    if(i > 0 || f > 0.0){
        p->cw++;
        return 1;
    }
    else{
        return 0;
    }
}
