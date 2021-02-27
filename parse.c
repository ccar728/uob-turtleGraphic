#include "parse.h"

int main(int argc, char** argv){
    int i;
    FILE* fp;
    Prog p;

    (void)argc;
    p.cw = 0;
    for(i = 0; i < MAXNUMTOKENS; i++){
        p.str[i][0] = '\0';
    }

    if(!(fp = fopen(argv[1], "r"))){
        fprintf(stderr, "Can't open %s\n", argv[1]);
        exit(2);
    }

    i = 0;
    while(fscanf(fp, "%s", p.str[i]) == 1 && i < MAXNUMTOKENS){
        i++;
    }
    fclose(fp);

    Main(&p);
    printf("Parsed OK!\n");

    return 0;
}

void Main(Prog* p){
    if(strcmp(p->str[p->cw], "{") == 0){
        p->cw++;
        Instrctlst(p);
        return;
    }
    ERROR("Expecting a open brace {");
}

void Instrctlst(Prog* p){
    if(strcmp(p->str[p->cw], "}") == 0){
        p->cw++;
        return;
    }
    Instruction(p);
    Instrctlst(p);
    return;
}

void Instruction(Prog* p){
    if(strcmp(p->str[p->cw], "FD") == 0){
        p->cw++;
        Fd(p);
    }
    else if(strcmp(p->str[p->cw], "RT") == 0){
        p->cw++;
        Rt(p);
    }
    else if(strcmp(p->str[p->cw], "LT") == 0){
        p->cw++;
        Lt(p);
    }
    else if(strcmp(p->str[p->cw], "DO") == 0){
        p->cw++;
        Do(p);
    }
    else if (strcmp(p->str[p->cw], "SET") == 0){
        p->cw++;
        Set(p);
    }
    else{
        ERROR("Illegal Instruction");
    }
}

void Fd(Prog* p){
    Varnum(p);
}

void Rt(Prog* p){
    Varnum(p);
}

void Lt(Prog* p){
    Varnum(p);
}

void Do(Prog *p){
    if(Var(p) == 0){
        ERROR("Expecting a Var after DO");
    }

    if(strcmp(p->str[p->cw], "FROM") != 0){
        ERROR("Expecting a FROM after Var in DO loop");
    }
    p->cw++;

    Varnum(p);

    if(strcmp(p->str[p->cw], "TO") != 0){
        ERROR("Expecting a TO after Var in DO loop");
    }
    p->cw++;

    Varnum(p);

    if(strcmp(p->str[p->cw], "{") != 0){
        ERROR("Expecting a { to start DO loop");
    }
    p->cw++;
    
    Instrctlst(p);
}

void Varnum(Prog* p){
    if(Num(p));
    else if(Var(p));
    else{
        ERROR("Illegal Varnum");
    }
}

void Set(Prog* p){
    if(Var(p) == 0){
        ERROR("Expecting a Var after SET");
    }

    if(strcmp(p->str[p->cw], ":=") != 0){
        ERROR("Expecting a := after Var in SET loop");
    }
    p->cw++;

    Polish(p);
}

void Polish(Prog* p){
    if(strcmp(p->str[p->cw], ";") == 0){
        p->cw++;
    }
    else if(Op(p)){
        Polish(p);
    }
    else{
        Varnum(p);
        Polish(p);
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
