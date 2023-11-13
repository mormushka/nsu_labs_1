#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {max_pattern_len = 16};

typedef struct TEXT_INFO{
    char str[max_pattern_len + 2];
    int len;
    unsigned hash;
    char bad;
} text_info;

typedef struct SEARCH_FIELD {
    text_info text_inf;
    size_t index;
    char first_char;
    FILE* in;
    unsigned mult_last_char;
} search_field;

int exp1(int num, int d) { return (d == 0) ? (1) : (num * exp1(num, d - 1)); }

unsigned hash_str(const char* str, int len) { //H(C0, ..., CN-1) = СУММА (Ci % 3) * 3^i
    unsigned hash = 0;
    for (int i = len - 1; i >= 0; --i)
        hash = hash * 3 + ((unsigned char)str[i] % 3);
    return hash;
}

text_info create_s_pattern(FILE* in) {
    text_info tmp = {.str = {0}};
    if (fgets(tmp.str, max_pattern_len + 2, in) == 0)  tmp.bad = 1; // +2 чтобы даже при макс длинне паттерна прочитать \n
    tmp.len = strlen(tmp.str) - 1;
    tmp.str[tmp.len] = '\0';
    tmp.hash = hash_str(tmp.str, tmp.len);
    return tmp;
}

search_field create_s_window(text_info* p, FILE* in) {
    search_field tmp = {.text_inf.str = {0}};
    if (fgets(tmp.text_inf.str, p->len + 1, in) == 0) tmp.text_inf.bad = 1;
    tmp.text_inf.hash = hash_str(tmp.text_inf.str, p->len);
    tmp.text_inf.len = p->len - 1;
    tmp.first_char = tmp.text_inf.str[0];
    tmp.mult_last_char = exp1(3, p->len - 1);
    tmp.in = in;
    return tmp;
}

void shift_hash( search_field* w ) {
    w->text_inf.hash = (w->text_inf.hash - ((unsigned char)w->first_char % 3)) / 3\
    + ((unsigned char)w->text_inf.str[w->text_inf.len] % 3) * w->mult_last_char;
}

char print_match(const text_info* p, const search_field* w) {
    for (int i = 0; i < p->len; ++i) {
        printf("%zu ", w->index + i + 1);
        if (p->str[i] != w->text_inf.str[i])  
            return 0;
    }
    return 1;
}

void move_forward(search_field* w) {
    memmove(w->text_inf.str, &w->text_inf.str[1], w->text_inf.len);
    w->text_inf.str[w->text_inf.len] = fgetc(w->in);

    shift_hash(w);
    w->first_char = w->text_inf.str[0];
    ++w->index;
}

void find_substring(FILE* in) {
    text_info p = create_s_pattern(in);
    search_field w = create_s_window(&p, in);

    printf("%u ", p.hash);
    
    if ((p.bad + w.text_inf.bad) > 0)
        exit(0);

    while (!feof(w.in)) {
        if (p.hash == w.text_inf.hash)
            print_match(&p, &w);
        move_forward(&w);
    }
}

int main()
{
    FILE* in = fopen("in.txt", "r");
    if (in == NULL) exit(0);

    find_substring(in);
    
    exit(0);
}