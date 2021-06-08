#include <stdio.h>
#include <math.h>
#include <omp.h>

void baseOne(int *arr, int n) {
    for (int i=0; i<n; i++) {
        arr[i]=1;
    }
}

int accumulate(int* first, int* last, int s) {
    int acc = s;

    for (int* i = first; i < last; i++) {
        acc += *i;
    }

    return acc;
}

int parallel_sum(int *arr, int arrSize) {
    int avaliable_threads = omp_get_max_threads(),
        subArrSize = (double)ceil((double)arrSize / avaliable_threads),
        *res = (int*)calloc(avaliable_threads, sizeof(int)),
        acc;

    #pragma omp parallel
    {
        int i, j, ID = omp_get_thread_num();

        for (i=ID*subArrSize, j=(i+subArrSize <= arrSize ? i+subArrSize : arrSize); i < j; i++) {
            res[ID] += arr[i];
        }
    }

    acc = accumulate(res, res+avaliable_threads, 0);

    free(res);

    return acc;
}


int main(int argc, char const *argv[])
{
    int arrSize = atoi(argv[1]),
        arr[arrSize];

    baseOne(arr, arrSize);

    printf("%d\n", parallel_sum(arr, arrSize));
    
    return 0;
}
