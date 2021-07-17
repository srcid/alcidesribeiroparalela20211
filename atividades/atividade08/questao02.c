// Correção: 0,5
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define swap(arr, a, b) {auto tmp=arr[a]; arr[a]=arr[b]; arr[b]=tmp;}

int compare(int a, int b) {
    return a < b ? -1 : a == b ? 0 : 1;
}

void par_qsort(int *data, int lo, int hi, int (*compare)(int a, int b))
{
    if (lo > hi) {
        return;
    }

    int l = lo,
        h = hi,
        pivot = data[(lo + hi) / 2];

    while (l <= h) {
        
        while (compare(data[l], pivot) < 0) {
            l++;
        }
        
        while (compare(data[h], pivot) > 0) {
            h--;
        }

        if (l <= h) {
            swap(data, l, h)
            l++;
            h--;
        }

        if (h - lo < 1000) {
            par_qsort(data, lo, h, compare);
        } else {
            #pragma omp task
            par_qsort(data, lo, h, compare);
        }

        if (hi - l < 1000) {
            par_qsort(data, l, hi, compare);
        } else {
            #pragma omp task
            par_qsort(data, l, hi, compare);
        }
    }
}

int *gen_arr(const int n) {
    int *arr = (int *) malloc(n*sizeof(int));

    srand(time(0));

    for (int i=0; i < n; i++) {
        arr[i]=rand()%100;
    }

    return arr;
}

void print_arr(int *arr, int n) {
    printf("[");
    for (int i=0; i < n-1; i++) {
        printf("%d, ", arr[i]);
    }
    printf("%d]\n", arr[n-1]);
}

int main(int argc, char const *argv[])
{
    int n;
    sscanf(argv[1], "%d", &n);

    int *arr = gen_arr(n);

    print_arr(arr, n);
    par_qsort(arr, 0, n-1, &compare);
    print_arr(arr, n);

    free(arr);    
    
    return 0;
}
