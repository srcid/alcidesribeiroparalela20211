#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

double** mtxAlloc(const int n) {
    double** m;
    int i;

    m = (double**)malloc(n*sizeof(double*));
    
    for (i=0; i<n; i++) {
        m[i] = (double*)malloc(n*sizeof(double));
    }

    return m;
}

void fillMtx(double** m, const int n, const int x) {
    int i, j;

    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            m[i][j]=x;
        }
    }
}

void mtxMulOutermost(double** c, double** a, double** b, const int n) {
    #pragma omp parallel for
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            c[i][j]=0;
            for (int k=0; k<n; k++) {
                c[i][j] += a[i][k]*b[k][j];
            }
        }
    }
}

void mtxMulCollapse(double** c, double** a, double** b, const int n) {
    #pragma omp parallel for collapse(2)
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            c[i][j]=0;
            for (int k=0; k<n; k++) {
                c[i][j] += a[i][k]*b[k][j];
            }
        }
    }
}

void mtxMulNested(double** c, double** a, double** b, const int n) {
    #pragma omp parallel for
    for (int i=0; i<n; i++) {    
        #pragma omp parallel for
        for (int j=0; j<n; j++) {
            c[i][j]=0;
            for (int k=0; k<n; k++) {
                c[i][j] += a[i][k]*b[k][j];
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    int n = atoi(argv[1]);
    int algo = atoi(argv[2]);
    double **a, **b, **c;

    vector<void (*)(double** c, double** a, double** b, const int n)> algos{
        &mtxMulOutermost, // 0
        &mtxMulCollapse,  // 1
        &mtxMulNested     // 2
    };

    a = mtxAlloc(n);
    b = mtxAlloc(n);
    c = mtxAlloc(n);

    fillMtx(a,n,1);
    fillMtx(b,n,1);
    fillMtx(c,n,1);

    algos[algo](c,a,b,n);

    return 0;
}
