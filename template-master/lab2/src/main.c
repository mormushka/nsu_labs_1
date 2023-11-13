#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { max_input = 10 };

void swap(char* a, char* b) { char tmp = *a; *a = *b; *b = tmp; }

int is_good(char* str, int len) {
    for (int i = 0; i < len; ++i) {
        if (!( ('0' <= str[i]) && (str[i] <= '9') )) { 
            return 0;
        }
        for (int j = i +1; j < len; ++j)
            if (str[i] == str[j])
                return 0;
    }
    return 1;
}

int perm(char* str, int len) {
    int i = -1, j = -1;
    for(int k = len - 2; k >= 0; --k) {
        if(str[k] < str[k + 1]) {
            i = k;
            break;
        }
    }
    if (i == -1) return 0;
    for(int k = len - 1; k >= 0; --k) {
        if(str[k] > str[i]) {
            j = k;
            break;
        }
    }
    if (j == -1) return 0;
    swap(&str[i], &str[j]);
    for (int k = i + 1, x = 1; x <= (len - i - 1) / 2; ++k, ++x) {
        swap(&str[k], &str[len - x]);
    }
    return 1;
}

int main()
{
    FILE* in = fopen("in.txt", "r");
    FILE* out = fopen("out.txt", "w");
    char str[max_input + 1] = {0};
    int x;
    char c;
    for(int i = 0;(c = fgetc(in)) != '\n'; ++i) {
        if (i == 10) { fprintf(out, "bad input"); exit(0);}
        str[i] = c;
    }
    int len = strlen(str);
    if (!is_good(str, len) || !fscanf(in, "%d", &x) ) { fprintf(out, "bad input"); exit(0);}

    for(int i = 0; i < x; ++i) {
        if(!perm(str, len)) break;
        fprintf(out, "%s\n", str);
    }

    exit(0);
}