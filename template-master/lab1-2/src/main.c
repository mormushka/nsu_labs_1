#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {max_p_len = 16};

#define BUFFER_SIZE  (max_p_len * 2)

typedef struct {
    char str[max_p_len + 2];
    int len;
    char bad;
    unsigned shift_table[max_p_len];
} search_pattern;

typedef struct {
    char data[BUFFER_SIZE];
    int head;
} rolling_buffer;

typedef struct {
    rolling_buffer r_buffer;
    size_t end_i; 
    char bad;
    FILE* in;
} search_field;

void fill_shift_table(search_pattern* p) {
    int i = 1; int j = 0;
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
    if (fgets(tmp.str, max_p_len + 2, in) == 0)  tmp.bad = 1; // +2 чтобы даже при макс длинне паттерна прочитать \n
    tmp.len = strlen(tmp.str) - 1;
    if (tmp.len == 0) tmp.bad = 1;
    fill_shift_table(&tmp);
    return tmp;
}

search_field create_s_window(FILE* in) {
    search_field tmp = {.r_buffer.data = {0}};
    for (int i = 0; i < BUFFER_SIZE; ++i) 
        tmp.r_buffer.data[i] = fgetc(in);
    tmp.end_i = BUFFER_SIZE - 1;
    tmp.in = in;
    return tmp;
}

char getc_window(search_field* w, size_t index) {
    if (index > w->end_i) {
        for (int i = 0; i < max_p_len; ++i) {
            w->r_buffer.data[w->r_buffer.head] = fgetc(w->in);
            w->r_buffer.head = (w->r_buffer.head + 1) % BUFFER_SIZE;
        }
        w->end_i += max_p_len;
    }
    return w->r_buffer.data[index % BUFFER_SIZE];
}

void find_substring(FILE* in) {
    search_pattern p = create_s_pattern(in);
    search_field w = create_s_window(in);

    if (p.bad) return;

    for (int i = 0; i < p.len; ++i)
        printf("%u ", p.shift_table[i]);

    size_t i = 0;
    unsigned j = 0;
    for(;;) {
        size_t x = i + p.len - j - 1;
        if (getc_window(&w, x) == EOF) return;

        if (p.str[j] == getc_window(&w, i)) {
            ++i; ++j;
        }
        else {
            if (j > 0) {
                printf("%zu %u ", i + 1 - j, j);
                j = p.shift_table[j - 1];
            }
            else
                ++i;
        }
    }

}

int main()
{
    FILE* in = fopen("in.txt", "r");
    if (in == NULL) exit(0);

    find_substring(in);

    exit(0);
}