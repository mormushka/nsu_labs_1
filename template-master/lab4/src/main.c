#include <stdio.h>
#include <stdlib.h>

#define LEN_OPPR 8

const struct {const char operator; const int priority;} op_prior[LEN_OPPR] = {
    ['+' % LEN_OPPR] = {'+', 1},
    ['-' % LEN_OPPR] = {'-', 1},
    ['*' % LEN_OPPR] = {'*', 2},
    ['/' % LEN_OPPR] = {'/', 2},
    ['(' % LEN_OPPR] = {'(', 0},
    [')' % LEN_OPPR] = {')', 0}
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

void show(stack* top) {
    stack* tmp = top;
    while (tmp != NULL) {
        printf("%d ", tmp->data);
        tmp = tmp->next;
    }
}

char suntax_is_normal(FILE* in) {
    char c;
    c = fgetc(in);
    while(!feof(in)) {
        if (!( (c >= '0') && (c <= '9') ))
            if (op_prior[c % LEN_OPPR].operator != c)
                return 0;
        c = fgetc(in);
    }
    return 1;
}

int calc(FILE* in, FILE* out) {
    stack* numbers = NULL;
    stack* operators = NULL; 

    if (!suntax_is_normal(in)) { 
        fprintf(out, "syntax error");
        exit(0);
    }

    
    
}

int main() {
    FILE* in = fopen("in.txt", "r");
    FILE* out = fopen("out.txt", "w");
    if ((in == NULL) || (out == NULL)) exit(0);
    calc(in, out);

}