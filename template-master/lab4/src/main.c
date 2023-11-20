#include <stdio.h>
#include <stdlib.h>

#define LEN_OPPR 8
#define END_IN(CHAR, IN) (((CHAR) == '\n') || feof(IN))
#define HESH(OBJ) (OBJ) % LEN_OPPR
#define PRIOR(OBJ) op_inf[(OBJ) % LEN_OPPR].priority
#define CALC_ON_STACK(NUMS, OPRS) push(&NUMS, op_inf[HESH(pick_up(&OPRS))].func(pick_up(&NUMS), pick_up(&NUMS)));

int f_plus     (int a, int b) { return a + b; }
int f_minus    (int a, int b) { return a - b; }
int f_myltiply (int a, int b) { return a * b; }
int f_division (int a, int b) { return a / b; }

const struct {const char operator; const int priority; int (*const func) (int, int);} op_inf[LEN_OPPR] = {
    [HESH('+')] = {'+', 1, f_plus},
    [HESH('-')] = {'-', 1, f_minus},
    [HESH('*')] = {'*', 2, f_myltiply},
    [HESH('/')] = {'/', 2, f_division},
    [HESH('(')] = {'(', 0},
    [HESH(')')] = {')', -1}
};

typedef struct STACK {
    int data;
    struct STACK* next;
} stack;

void push(stack** top, int data) {
    stack* tmp = malloc(sizeof(stack));
    tmp->data = data;
    tmp->next = *top;
    *top = tmp;
}

void pop(stack** top) {
    if (*top == NULL)
        return;
    stack* tmp = (*top)->next;
    free(top);
    *top = tmp;
}

int pick_up(stack** top) {
    int tmp = (*top)->data;
    pop(top);
    return tmp;
}

void show(stack* top, char* x) {
    stack* tmp = top;
    while (tmp != NULL) {
        printf(x, tmp->data);
        tmp = tmp->next;
    }
}

char suntax_is_normal(FILE* in) {
    char c;
    c = fgetc(in);
    while(!END_IN(c, in)) {
        if (!( (c >= '0') && (c <= '9') ))
            if (op_inf[c % LEN_OPPR].operator != c)
                return 0;
        c = fgetc(in);
    }
    fseek(in, 0, SEEK_SET);
    return 1;
}

void get_next_object(FILE* in, int* obj, char* it_num) {
    char c = fgetc(in);
    if ((c >= '0') && (c <= '9')) {
        *it_num = 1;
        *obj = 0;
        
        do {
            *obj *= 10;
            *obj += c - '0';
        } while (((c = fgetc(in)) >= '0') && (c <= '9'));
        
        if(!END_IN(c, in))
            fseek(in, ftell(in) - 1, SEEK_SET);
    }
    else {
        *it_num = 0;
        *obj = c;
    }
}

int calc(FILE* in, FILE* out) {
    stack* numbers = NULL;
    stack* operators = NULL; 

    if (!suntax_is_normal(in)) { 
        fprintf(out, "syntax error");
        exit(0);
    }

    char it_num;
    int obj;
    get_next_object(in, &obj, &it_num);
    // no brackets yet
    for (;;) {
        if (it_num)
            push(&numbers, obj);
        else {
            if (END_IN(obj, in)) {
                while (operators != NULL)
                    CALC_ON_STACK(numbers, operators);
                break;
            }
            else {
                while ((operators != NULL) && (PRIOR(obj) <= PRIOR(operators->data)))
                    CALC_ON_STACK(numbers, operators);
                push(&operators, obj);
            }
        }
        get_next_object(in, &obj, &it_num);
    }

    show(numbers, "%d ");
    show(operators, "%c ");
}

int main() {
    FILE* in = fopen("in.txt", "r");
    FILE* out = fopen("out.txt", "w");
    if ((in == NULL) || (out == NULL)) exit(0);
    calc(in, out);

}