#include <bits/stdc++.h>
#include <omp.h>
#include <immintrin.h>

using namespace std;

unsigned int *genArr(const unsigned int& n) {
    unsigned int *arr = (unsigned int *)malloc(sizeof(unsigned int)*n);

    #pragma omp parallel for
    for (unsigned int i=0; i<n; i++) {
        _rdrand32_step(&arr[i]);
    }

    return arr;
}

unsigned int getMax(unsigned int *arr, const unsigned int& n) {
    unsigned int cur = arr[0];

    for (unsigned int i=1; i<n; i++) {
        if (arr[i] > cur) {
            cur = arr[i];
        }
    }

    return cur;
}

unsigned int getMaxParallel(unsigned int *arr, const unsigned int& n) {
    unsigned int maxVal = arr[0];

    #pragma omp parallel for reduction(max:maxVal)
    {
        for (unsigned int i=0; i<n; i++) {
            if (maxVal < arr[i]) {
                maxVal=arr[i];
            }
            
        }
    }

    return maxVal;
}

int main(int argc, char const *argv[])
{   
    const unsigned int n = atoi(argv[1]);
    unsigned int *arr = genArr(n);
    
    auto start = chrono::high_resolution_clock::now();
    cout << "Maior número (sequêncial): " << getMax(arr, n) << endl;
    auto stop = chrono::high_resolution_clock::now();
    auto dur = chrono::duration_cast<chrono::nanoseconds>(stop - start);
    cout << "Tempo (ns): " << dur.count() << endl;
    start = chrono::high_resolution_clock::now();
    cout << "Maior número (paralela): " << getMaxParallel(arr, n) << endl;
    stop = chrono::high_resolution_clock::now();
    dur = chrono::duration_cast<chrono::nanoseconds>(stop - start);
    cout << "Tempo (ns): " << dur.count() << endl;

    free(arr);

    return 0;
}
