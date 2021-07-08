#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

long fib(long n)
{
    return n < 2 ? 1 : fib(n-1) + fib(n-2);
}

void asc_fib(const int max) {
    #pragma omp parallel
    #pragma omp single
    for (int n = 1; n <= max; n++)
        #pragma omp task
        fib(n); //printf("%d: %d %ld\n", omp_get_thread_num(), n, fib(n));
}

void dsc_fib(const int max) {
    #pragma omp parallel
    #pragma omp single
    for (int n = max; n >= 1; n--)
        #pragma omp task
        fib(n); //printf("%d: %d %ld\n", omp_get_thread_num(), n, fib(n));

}


int main(int argc, char const *argv[])
{
    const int max = atoi(argv[1]);
    const int strategy = atoi(argv[2]);
    void (*funcs[2])(const int max);
    funcs[0] = &asc_fib;
    funcs[1] = &dsc_fib;

    funcs[strategy](max);

    return 0;
}


