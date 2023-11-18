#include <stdio.h>
#include <stdlib.h>

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

int main() {
    stack* s = NULL;
    push(&s, 1);
    push(&s, 2);
    show(s);
    printf("\n");
    pop(&s); 
    show(s);
}