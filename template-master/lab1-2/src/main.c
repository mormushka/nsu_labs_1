#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_P_LEN  16
#define BUFFER_SIZE 20

typedef struct {
    char str[MAX_P_LEN + 2];
    unsigned len;
    char bad;
    unsigned shift_table[MAX_P_LEN];
} search_pattern;

typedef struct {
    size_t* data;
    size_t first;
    size_t free_block;
    int count;
} queue;

queue* create_queue(int size) {
    queue* tmp = (queue*)malloc(sizeof(queue));
    tmp->data = (size_t*)malloc(sizeof(size_t) * 2 * size);
    tmp->first = tmp->free_block = tmp->count = 0;
    return tmp;
}

void queue_add(queue* q, size_t index, size_t prefix_len) {
    q->data[(q->free_block++) % BUFFER_SIZE] = index;
    q->data[(q->free_block++) % BUFFER_SIZE] = prefix_len;
    q->count++;
}

void del_first(queue* q) {
    q->first = (q->first + 2) % BUFFER_SIZE;
    q->count--;
}

void fill_shift_table(search_pattern* p) {
    unsigned i = 1; unsigned j = 0;
    while (p->len > i) {
        if (p->str[j] == p->str[i]) {
            p->shift_table[i] = ++j; ++i;
        }
        else {
            if (j == 0) {
                p->shift_table[i] = 0; ++i;
            }
            else
                j = p->shift_table[j - 1];
        }
    } 
}

search_pattern create_s_pattern(FILE* in) {
    search_pattern tmp = {.str = {0}};
    if (fgets(tmp.str, MAX_P_LEN + 2, in) == 0) tmp.bad = 1; // +2 чтобы даже при макс длинне паттерна прочитать \n
    tmp.len = strlen(tmp.str) - 1;
    if (tmp.len == 0) tmp.bad = 1;
    fill_shift_table(&tmp);
    return tmp;
}

void find_substring(FILE* in) {
    search_pattern p = create_s_pattern(in);

    for (unsigned i = 0; i < p.len; ++i)
        printf("%u ", p.shift_table[i]);

    if (p.bad) return;

    queue* q = create_queue(BUFFER_SIZE);
    size_t i = 0;
    unsigned j = 0;
    char c = fgetc(in);
    while(!feof(in)) {
        if (p.str[j] == c) {
            ++i; ++j;
            c = fgetc(in);
        }
        else {
            if (j > 0) {
                queue_add(q, i + 1 - j, j);
                j = p.shift_table[j - 1];
            }
            else {
                ++i;
                c = fgetc(in);
            }
        }
        while ((q->data[q->first] <= i - p.len + 1) && q->count) {
            printf("%zu %zu ", q->data[q->first], q->data[q->first + 1]);
            del_first(q);
        }
    }

    if (j == p.len) {
        queue_add(q, i + 1 - j, j);
    }
    while ((q->data[q->first] <= i - p.len + 1) && q->count) {
        printf("%zu %zu ", q->data[q->first], q->data[q->first + 1]);
        del_first(q);
    }

    free(q->data);
    free(q);
}

int main()
{
    FILE* in = fopen("in.txt", "r");
    if (in == NULL) exit(0);

    find_substring(in);

    exit(0);
}