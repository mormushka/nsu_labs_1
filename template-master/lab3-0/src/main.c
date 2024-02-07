#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b) { 
    int tmp = *a; 
    *a = *b; 
    *b = tmp; 
}

void heapify(int* a, const int n, const int i) {
	int largest = i;

	int left = 2*i + 1;
	int right = 2*i + 2;

	if ((left < n) && (a[left] > a[largest]))
		largest = left;
	if ((right < n) && (a[right] > a[largest]))
		largest = right;

	if (largest != i) {
		swap(&a[i], &a[largest]);
		heapify(a, n, largest);
	}
}

void heap_sort(int* a, const int n) {
	// build max heap
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(a, n, i);

	// heap sort
	for (int i = n - 1; i >= 0; i--) {
		swap(&a[0], &a[i]);
		heapify(a, i, 0);
	}
}

int main()
{
    size_t n;
    if (!scanf("%zu", &n))
        return 0;

    int* a = (int *)malloc(n * sizeof(int));
    for (size_t i = 0; i < n; ++i) {
        if(!scanf("%d", &a[i])) {    
            free(a);
            return 0;
        }
    }
    
    heap_sort(a, n);

    for(size_t i = 0; i < n; ++i) 
        printf("%d ", a[i]);
    
    free(a);
    return 0;
}