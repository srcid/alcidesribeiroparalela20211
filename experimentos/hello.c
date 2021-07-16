// Legal manter uma pasta separada para testes.
#include <stdio.h>
#include <omp.h>

int main(int argc, char const *argv[])
{
    #pragma omp parallel
    {
        int ID = omp_get_thread_num();
        printf("Hello(%d)", ID);
        printf("World(%d)\n", ID);
    }

    return 0;
}
