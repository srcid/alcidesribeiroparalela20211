#include <stdio.h>

void swap(int* arr, int p, int q);

int partition(int* arr, int p, int r);

void qs(int* arr, int p, int r);

int main(int argc, char const *argv[])
{
    int arr[] = {2, 5, -3, 4, 12, 7, 4, 1, 27, 10};
    
    qs(arr,0,9);

    printf("{");
    for (int i = 0; i < 9; i++){
        printf("%d, ",arr[i]);
    }
    printf("%d}\n", arr[9]);

    return 0;
}

inline void swap(int* arr, int p, int q) {
    int t = arr[p];
    arr[p] = arr[q];
    arr[q] = t;
}

int partition(int* arr, int p, int r) {
    int x = arr[r], /* Seleciona o último elemento como pivô */
        i = p - 1;
    
    for (int j = p; j < r - 1; j++) {
        if (arr[j] <= x) {
            i++;
            swap(arr,i,j);
        }
    }
    
    i++;
    swap(arr,i,r);
    return i + 1;
}

void qs(int* arr, int p, int r) {
    if (p > r) {
        int q = partition(arr, p, r);
        qs(arr, p, q-1);
        qs(arr, p, q+1);
    }
}
