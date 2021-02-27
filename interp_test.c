/*
    Fd(), Lt(), Rt(), Do(), Set(), Polish(), Varnum(), is_Num(), is_Op(), is_Num()
    have been tested in parse_test.c
*/
#include "interp_test.h"

int main(void){
    int i;
    FILE* fp;
    Prog p;
    Stack s;
    char ch;

    p.cw = 0;
    for(i = 0; i < 100; i++){
        p.str[i][0] = '\0';
        p.num_list[0] = p.var_list[0] = 0.0;
    }
    assert(strcmp(p.str[p.cw], "") == 0);

    if(!(fp = fopen(PROGNAME, "r"))){
        fprintf(stderr, "Can't open %s\n", PROGNAME);
        exit(2);
    }

    i = 0;
    while(fscanf(fp, "%s", p.str[i]) == 1 && i < 100){
        i++;
    }
    fclose(fp);

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
    p.cw = 0;

    /* test stack: INIT(), PUSH(), POP(), FREE() */
    stack_INIT(&s);
    assert(s.num == 0);
    assert(s.top->next == NULL);
    PUSH(&s, "A");
    assert(s.top->c = 'A');
    assert(s.num == 1);
    ch = POP(&s);
    assert(ch = 'A');
    assert(s.num == 0);
    FREE(&s);
    assert(s.top == NULL);

    /* test char_to_i(), str_ro_c(), str_to_f() */
    assert(char_to_i('B') == 1);
    p.cw = 2;
    assert(strcmp(p.str[p.cw], "A") == 0);
    assert(str_to_c(&p) == 'A');
    p.cw = 12;
    assert(strcmp(p.str[p.cw], "1.5") == 0);
    assert(str_to_f(&p) == 1.5);

    /* test get_operand() */
    assert(test_get(1.55, 'B') == 1);

    /* test polish_cal() */
    p.cw = 13;
    assert(strcmp(p.str[p.cw], "*") == 0);
    /* 2.42 is not exactly equal to 2.42 when using float */
    assert(test_cal(&p) > 2.4 && test_cal(&p) < 2.43); 

    printf("Interpreer test OK!\n");
    return 0;
}

double test_cal(Prog* p){
    double oper1 = 1.1, oper2 = 2.2, res;
    char op = p->str[p->cw][0];
    
    if(op == '+'){
        return res = oper1 + oper2;
    }
    else if(op == '-'){
        return res = oper1 - oper2;
    }
    else if(op == '*'){
        return res = oper1 * oper2;
    }
    else if(op == '/'){
        return res = oper1 / oper2;
    }
    else{
        return res = error;
    }
    return res;
}

int test_get(double f_num, char ch){
    char oper[2];
    int data = 0;
    int var_to_num;

    oper[0] = ch;
    oper[1] = '\0';

    if(atoi(oper)){
        data = f_num;
    }
    else if(isupper(oper[0])){
        var_to_num = oper[0] - 'A';
        data = var_to_num;
    }
    return data;
}

int char_to_i(char var){
    return var - 'A';
}

double str_to_f(Prog* p){
    return atof(p->str[p->cw]);
}

char str_to_c(Prog* p){
    char var = p->str[p->cw][0];
    if(isupper(var)){
        return var;
    }
    return 0;
}

void stack_INIT(Stack* s){
    s->top = (Node*)malloc(sizeof(Node));
    s->top->next = NULL;
    s->num = 0;
}

void PUSH(Stack* s, char* ch){
    Node* node = (Node*)malloc(sizeof(Node));
    node->next = s->top;
    node->next->c = *ch;
    s->top = node;
    s->num++;
}

char POP(Stack* s){
    free(s->top);
    s->top = s->top->next;
    s->num--;
    if(s->top == NULL){
        return '\0';
    }
    return s->top->c;
}

void FREE(Stack* s){
    while(s->top != NULL){
        free(s->top);
        s->top = s->top->next;
    }
}
