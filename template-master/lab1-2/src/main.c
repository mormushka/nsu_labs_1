#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_P_LEN  16
#define BUFFER_SIZE MAX_P_LEN

typedef struct {
    char str[MAX_P_LEN + 2];
    int len;
    char bad;
    unsigned shift_table[MAX_P_LEN];
} search_pattern;

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
    if (fgets(tmp.str, MAX_P_LEN + 2, in) == 0) tmp.bad = 1; // +2 чтобы даже при макс длинне паттерна прочитать \n
    tmp.len = strlen(tmp.str) - 1;
    if (tmp.len == 0) tmp.bad = 1;
    fill_shift_table(&tmp);
    return tmp;
}

void find_substring(FILE* in) {
    search_pattern p = create_s_pattern(in);

    for (int i = 0; i < p.len; ++i)
        printf("%u ", p.shift_table[i]);

    if (p.bad) return;

    size_t i = 0;
    unsigned j = 0;
    char c = fgetc(in);
    int x[2] = {0};
    while(!feof(in)) {
        if (p.str[j] == c) {
            ++i; ++j;
            c = fgetc(in);
        }
        else {
            if (j > 0) {
                if (x[1]) {
                    printf("%zu %u ", x[0], x[1]);
                    x[0] = i + 1 - j;
                    x[1] = j;
                }
                else {
                    x[0] = i + 1 - j;
                    x[1] = j;
                }
                j = p.shift_table[j - 1];
            }
            else {
                ++i;
                c = fgetc(in);
            }
        }
    }
    if (j == p.len) {
        if (x[1]) printf("%zu %u ", x[0], x[1]);
        printf("%zu %u ", i + 1 - j, j);
    }
    if (x[1] == p.len) {
        printf("%zu %u ", x[0], x[1]);
    }
}

int main()
{
    FILE* in = fopen("in.txt", "r");
    if (in == NULL) exit(0);

    find_substring(in);

    exit(0);
}