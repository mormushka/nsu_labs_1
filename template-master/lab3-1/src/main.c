#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void quick_sort_m(int* a, size_t l, size_t r) {
    if (r > l) {
        int p = a[rand() % (r - l + 1) + l];
        size_t i = l, j = r;
        while ((i <= j) && (j <= r)) {
            while (a[i] < p) i++;
            while ((a[j] > p) && (j <= r)) j--;
            if ((i <= j) && (j <= r)) {
                swap(&a[i], &a[j]);
                i++;
                j--;
            }
        }
        quick_sort_m(a, i, r);
        if (j > r) return;
        quick_sort_m(a, l, j);
    }
}

void quick_sort(int* a, size_t len) {
    srand(time(NULL));
    if (len == 0) return;
    quick_sort_m(a, 0, len - 1);
}

int main()
{
    FILE* in = fopen("in.txt", "r");
    FILE* out = fopen("out.txt", "w");

    size_t len;
    if (!fscanf(in, "%zu", &len)) {
        fclose(in);
        fclose(out);
        return 0;
    }

    int* a = (int *)malloc(len * sizeof(int));
    for (size_t i = 0; i < len; ++i) {
        if(!fscanf(in, "%d", &a[i])) {    
            free(a);
            fclose(in);
            fclose(out);
            return 0;
        }
    }
    
    quick_sort(a, len);

    for(size_t i = 0; i < len; ++i) 
        fprintf(out, "%d ", a[i]);
    
    free(a);
    fclose(in);
    fclose(out);
    return 0;
}