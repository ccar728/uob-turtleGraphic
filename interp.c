#include "interp.h"

int main(int argc, char* argv[]){
    int i;
    FILE* fp;
    Prog p, l;
    SDL_Simplewin sw;
    Coordinate crd;
    Stack s;

    (void)argc;

    p.cw = 0;
    for(i = 0; i < MAXNUMTOKENS; i++){
        p.str[i][0] = '\0';
        p.num_list[0] = p.var_list[0] = 0.0;
    }

    if(!(fp = fopen(argv[1], "r"))){
        fprintf(stderr, "Cannot open %s\n", argv[1]);
        exit(2);
    }

    i = 0;
    while(fscanf(fp, "%s", p.str[i]) == 1 && i < MAXNUMTOKENS){
        i++;
    }
    fclose(fp);

    stack_INIT(&s);
    drawing_INIT(&sw, &crd, &l);

    Main(&p, &s, &sw, &crd, &l);
    FREE(&s);

    SDL_Delay(PAUSE_TIME);
    SDL_Quit();
    printf("Interpreted OK!!\n");

    return 0;
}

void drawing_INIT(SDL_Simplewin* sw, Coordinate* crd, Prog* l){
    int i;
    crd->x1 = X0;
    crd->y1 = Y0;
    crd->angle = ANGLE0;

    l->cw = 0;
    for(i = 0; i < ARR_SIZE; i++){
        l->str[i][0] = 0;
        l->num_list[i] = 0;
        l->var_list[i] = 0;
    }

    Neill_SDL_Init(sw);
    /* (white) R:255, G:255, B:255 */
    Neill_SDL_SetDrawColour(sw, R, G, B);
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
    if(s->top == NULL){
        return '\0';
    }
    else{
        free(s->top);
        s->top = s->top->next;
        s->num--;
        return s->top->c;
    }
}

void FREE(Stack* s){
    while(s->top != NULL){
        free(s->top);
        s->top = s->top->next;
    }
}

void Main(Prog* p, Stack* s, SDL_Simplewin* sw, Coordinate* crd, Prog* l){
    if(strcmp(p->str[p->cw], "{") == 0){
        p->cw++;
        Instrctlst(p, s, sw, crd, l);
        return;
    }
    ERROR("Expecting a open brace {");
}

void Instrctlst(Prog* p, Stack* s, SDL_Simplewin* sw, Coordinate* crd, Prog* l){
    if(strcmp(p->str[p->cw], "}") == 0){
        p->cw++;
        return;
    }
    Instruction(p, s, sw, crd, l);
    Instrctlst(p, s, sw, crd, l);
    return;
}
void Instruction(Prog* p, Stack* s, SDL_Simplewin* sw, Coordinate* crd, Prog* l){
    if(strcmp(p->str[p->cw], "FD") == 0){
        p->cw++;
        Fd(p, sw, crd, l);
    }
    else if(strcmp(p->str[p->cw], "LT") == 0){
        p->cw++;
        Lt(p, crd, l);
    }
    else if(strcmp(p->str[p->cw], "RT") == 0){
        p->cw++;
        Rt(p, crd, l);
    }
    else if(strcmp(p->str[p->cw], "DO") == 0){
        p->cw++;
        Do(p, s, sw, crd, l);
    }
    else if(strcmp(p->str[p->cw], "SET") == 0){
        p->cw++;
        Set(p, s, crd, l);
    }
    else if(strcmp(p->str[p->cw], "}") == 0){
        ;
    }
    else{
        ERROR("Illegal Instruction");
    }
}

void Fd(Prog* p, SDL_Simplewin* sw, Coordinate* crd, Prog* l){
    double data;
    char var = str_to_c(p);
    int var_num = char_to_i(var);

    if(VarNum(p) == TYPE_NUM){
        data = str_to_f(p);
        p->cw++;
    }
    else{
        data = l->var_list[var_num];
        p->cw++;
    }

    crd->x0 = crd->x1;
    crd->y0 = crd->y1;

    crd->x1 = crd->x1 + data * cos(crd->angle);
    crd->y1 = crd->y1 + data * sin(crd->angle);

    SDL_RenderDrawLine(sw->renderer, crd->x0, crd->y0, crd->x1, crd->y1);
    Neill_SDL_UpdateScreen(sw);
    Neill_SDL_Events(sw);
    SDL_Delay(GAP);
}

void Lt(Prog* p, Coordinate* crd, Prog* l){
    double data;
    char var;
    int var_num;
    if(VarNum(p) == TYPE_NUM){
        data = str_to_f(p);
    }
    else{
        var = str_to_c(p);
        var_num = char_to_i(var);
        data = l->var_list[var_num];
    }
    crd->angle = crd->angle - data * (PI / PI_DEG);
    p->cw++;
}

void Rt(Prog* p, Coordinate* crd, Prog* l){
    double data;
    char var;
    int var_num;
    if(VarNum(p) == TYPE_NUM){
        data = str_to_f(p);
    }
    else{
        var = str_to_c(p);
        var_num = char_to_i(var);
        data = l->var_list[var_num];
    }
    crd->angle = crd->angle + data * (PI / PI_DEG);
    p->cw++;
}

int Do(Prog* p, Stack* s, SDL_Simplewin* sw, Coordinate* crd, Prog* l){
    int row, res;
    double num1, num2, min, max;
    char letter;
    char var = str_to_c(p);
    int var_to_num = char_to_i(var);

    p->cw++;/* jump the var after DO */

    if(strcmp(p->str[p->cw], "FROM") != 0){
        ERROR("Expecting a FROM after Var in DO loop");
    }
    p->cw++;

    /* Varnum() */
    if (VarNum(p) == TYPE_NUM){
        min = num1 = str_to_f(p);
        l->var_list[var_to_num] = min;
    }
    else{
        letter = str_to_c(p);
        res = char_to_i(letter);
        min = l->var_list[res];
        l->var_list[var_to_num] = min;
    }
    p->cw++;

    if(strcmp(p->str[p->cw], "TO") != 0){
        ERROR("Expecting a TO after Var in DO loop");
    }
    p->cw++;

    if (VarNum(p) == TYPE_NUM){
        max = num2 = str_to_f(p);
    }
    else{
        letter = str_to_c(p);
        res = char_to_i(letter);
        max = l->var_list[res];
    }
    p->cw++;

    if(strcmp(p->str[p->cw], "{") != 0){
        ERROR("Expecting a { to start DO loop");
    }
    p->cw++;

    while(l->var_list[var_to_num] <= max){
        row = p->cw;
        Instrctlst(p, s, sw, crd, l);
        l->var_list[var_to_num]++;
        if(l->var_list[var_to_num] <= max){
            p->cw = row;
        }
    }
    return 1;
}

void Set(Prog* p, Stack* s, Coordinate* crd, Prog* l){
    char var = str_to_c(p);
    int var_num = char_to_i(var);

    p->cw++;

    if(strcmp(p->str[p->cw], ":=") != 0){
        ERROR("Expecting a := after Var in SET loop");
    }
    p->cw++;

    l->var_list[var_num] = Polish(p, s, crd, l);
}

double Polish(Prog* p, Stack* s, Coordinate* crd, Prog* l){
    char buff[ARR_SIZE];
    double res;

    if(strcmp(p->str[p->cw], ";") == 0 && s->num != 0){
        POP(s);
        res = l->num_list[s->num];
        for(; s->num > 0;){
            POP(s);
        }
        p->cw++;
        return res;
    }
    else if(is_Op(p)){
        if(s->top != NULL && s->num >= 2){
            l->num_list[s->num] = Polish_cal(s, l, is_Op(p));
            sprintf(buff, "%f", l->num_list[s->num]);
            PUSH(s, buff);
            p->cw++;
            return Polish(p, s, crd, l);
        }
        else{
            ERROR("Expecting Two Operands");
        }
    }
    else if(VarNum(p) == TYPE_VAR){
        PUSH(s, p->str[p->cw]);
        p->cw++;
        return Polish(p, s, crd, l);
    }
    else if(VarNum(p) == TYPE_NUM){
        double num = str_to_f(p);
        l->num_list[s->num] = num;
        PUSH(s, p->str[p->cw]);
        p->cw++;
        return Polish(p, s, crd, l);
    }
    return 0;
}

double Polish_cal(Stack* s, Prog* l, type op){
    double res, oper1, oper2;
    oper2 = get_operand(s, l);
    oper1 = get_operand(s, l);

    if(op == add){
        return res = oper1 + oper2;
    }
    else if(op == subs){
        return res = oper1 - oper2;
    }
    else if(op == multi){
        return res = oper1 * oper2;
    }
    else if(op == divi){
        return res = oper1 / oper2;
    }
    else{
        return res = error;
    }
}

double get_operand(Stack* s, Prog* l){
    char oper[2]; /* currnet operand and '\0' */
    oper[0] = POP(s);
    oper[1] = '\0';

    if(isdigit(oper[0])){
        return l->num_list[s->num];
    }
    else if(isupper(oper[0])){
        return l->var_list[oper[0] - 'A'];
    }
    return 0;
}

int VarNum(Prog* p){
    if(is_Num(p) == true) {
        return TYPE_NUM;
    }
    if(is_Var(p) == true) {
        return TYPE_VAR;
    }
    ERROR("Expecting a Number or Letter");
}

bool is_Var(Prog* p){
    char var = p->str[p->cw][0];
    if(isupper(var)){
        return true;
    }
    return false;
}

bool is_Num(Prog* p){
    int i = atoi(p->str[p->cw]);
    float f = atof(p->str[p->cw]);

    if(i > 0 || f > 0.0 || isdigit(p->str[p->cw][0])){
        return true;
    }
    else{
        return false;
    }
}

type is_Op(Prog* p){
    char type = 0;
    char op = p->str[p->cw][0];
    if(op == '+'){
        type = add;
    }
    else if(op == '-'){
        type = subs;
    }
    else if(op == '*'){
        type = multi;
    }
    else if(op == '/'){
        type = divi;
    }
    else{
        type = error;
    }
    return type;
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
