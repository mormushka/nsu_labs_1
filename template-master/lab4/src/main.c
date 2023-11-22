#include <stdio.h>
#include <stdlib.h>

#define O2X {0,f_NAO},{0,f_NAO},
#define O8X O2X O2X O2X O2X
#define O32X O8X O8X O8X O8X
#define O128X O32X O32X O32X O32X
#define O256X O128X O128X

#define STOP_WORK(TEXT) { printf(TEXT); exit(0); }
#define LEN_OPPR 256
#define END_IN(CHAR, IN) (((CHAR) == '\n') || feof(IN))
#define PRIOR(OBJ) op_inf[(OBJ) % LEN_OPPR].priority
#define CALC_ON_STACK(NUMS, OPRS) { \
    if ((NUMS == NULL) || (NUMS->next == NULL)) STOP_WORK("syntax error");\
    if (OPRS == NULL) STOP_WORK("syntax error");\
    push(&NUMS, op_inf[pick_up(&OPRS)].func(pick_up(&NUMS), pick_up(&NUMS))); \
}

int f_plus     (int a, int b) { return a + b; }
int f_minus    (int a, int b) { return a - b; }
int f_myltiply (int a, int b) { return a * b; }
int f_division (int a, int b) { if (!b) STOP_WORK("division by zero"); 
                                return a / b; }
int f_NAO      (int a, int b) { STOP_WORK("syntax error"); }

const struct name{const int priority; int (*const func) (int, int);} op_inf[LEN_OPPR] = {
    O256X
    ['+'] = {1, f_plus},
    ['-'] = {1, f_minus},
    ['*'] = {2, f_myltiply},
    ['/'] = {2, f_division},
    ['('] = {0, NULL},
    [')'] = {0, NULL}
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
    free(*top);
    *top = tmp;
}

int pick_up(stack** top) {
    int tmp = (*top)->data;
    pop(top);
    return tmp;
}

void get_next_object(FILE* in, int* obj, char* it_num) {
    char c = *obj = fgetc(in);
    *it_num = ((c) >= '0') && (c <= '9');
    if (*it_num) {
        *obj = 0;
        do {
            *obj *= 10;
            *obj += c - '0';
        } while (((c = fgetc(in)) >= '0') && (c <= '9'));
        
        if(!END_IN(c, in))
            fseek(in, ftell(in) - 1, SEEK_SET);
    }
}

int calc(FILE* in) {
    stack* numbers = NULL;
    stack* operators = NULL;

    char it_num;
    int obj;
    get_next_object(in, &obj, &it_num);
    for (;;) {
        if (it_num)
            push(&numbers, obj);
        else {
            if (END_IN(obj, in)) {
                while (operators != NULL)
                    CALC_ON_STACK(numbers, operators);
                break;
            }
            else if (obj == '(') {
                push(&operators, obj);
                get_next_object(in, &obj, &it_num);
                if (obj == ')') STOP_WORK("syntax error");
                continue;
            }
            else if (obj == ')') {
                if (operators == NULL) STOP_WORK("syntax error");
                while (operators->data != '(') {
                    CALC_ON_STACK(numbers, operators);
                    if (operators == NULL) STOP_WORK("syntax error");
                }
                pop(&operators);
            }
            else {
                while ((operators != NULL) && (PRIOR(obj) <= PRIOR(operators->data)))
                    CALC_ON_STACK(numbers, operators);
                push(&operators, obj);
            }
        }
        get_next_object(in, &obj, &it_num);
    }
    if (numbers == NULL)
        STOP_WORK("syntax error");
    return numbers->data;
}

int main() {
    FILE* in = fopen("in.txt", "r");
    if (in == NULL) exit(0);

    printf("%d", calc(in));
    exit(0);
}