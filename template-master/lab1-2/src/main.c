#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {max_p_len = 16, w_len = max_p_len * 2};

typedef struct SEARCH_PATTERN {
    char str[max_p_len + 2];
    int len;
    char bad;
    unsigned shift_table[max_p_len];
} search_pattern;

typedef struct SEARCH_FIELD {
    char str[w_len];
    size_t st_i, end_i;
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
    tmp.str[tmp.len] = '\0';
    fill_shift_table(&tmp);
    return tmp;
}

search_field create_s_window(FILE* in) {
    search_field tmp = {.str = {0}};
    for (int i = 0; i < w_len; ++i) tmp.str[i] = fgetc(in);
    tmp.st_i = 0; tmp.end_i = w_len - 1;
    tmp.in = in;
    return tmp;
}

char getc_window(search_field* w, size_t index) {
    if (index > w->end_i) {
        memmove(w->str, &w->str[max_p_len], max_p_len);
        for (int i = 0; i < max_p_len; ++i)
            w->str[max_p_len + i] = fgetc(w->in);
        w->st_i += max_p_len; w->end_i += max_p_len;
    }
    return w->str[index - w->st_i];
}

void find_substring(FILE* in) {
    search_pattern p = create_s_pattern(in);
    search_field w = create_s_window(in);

    for (int i = 0; i < p.len; ++i)
        printf("%u ", p.shift_table[i]);
    
    if (p.bad) return;

    size_t i = 0;
    unsigned j = 0;
    for(;;) {
        size_t x = i + p.len - j - 1;
        if (getc_window(&w, x) == EOF) 
            return;

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