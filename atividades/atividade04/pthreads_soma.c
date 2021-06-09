#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <math.h>

void baseZero(int* arr, int n) {
    for (int i=0; i<n; i++) {
        arr[i]=0;
    }
}

void baseOne(int *arr, int n) {
    for (int i=0; i<n; i++) {
        arr[i]=1;
    }
}

struct partToSum {
    int *arr,
        arrSize,
        subArrSize,
        *res,
        ID;
};

void* sum(void* s_ptr) {
    struct partToSum *part = (struct partToSum *)s_ptr;

    int i, j;

    for (i=part->ID*part->subArrSize, j=(i+part->subArrSize <= part->arrSize ? i+part->subArrSize : part->arrSize); i < j; i++) {
        part->res[part->ID] += part->arr[i];
    }

    return NULL;
}

int parallel_sum(int* arr, int arrSize) {

    int numberOfThreads = get_nprocs(),
        subArrSize = (int)ceil((double)arrSize/(double)numberOfThreads),
        res[numberOfThreads],
        acc = 0;

    pthread_t threads[numberOfThreads];
    struct partToSum part[numberOfThreads];
    
    baseZero(res, numberOfThreads);

    for (int i=0; i < numberOfThreads; i++) {
        part[i].arr = arr;
        part[i].arrSize = arrSize;
        part[i].subArrSize = subArrSize;
        part[i].res = res;
        part[i].ID = i;

        pthread_create(&threads[i], NULL, sum, (void *)&part[i]);
    }

    for (int i=0; i < numberOfThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i=0; i < numberOfThreads; i++) {
        acc += res[i];
    }
    
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
