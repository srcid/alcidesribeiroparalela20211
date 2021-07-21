// Correção: 1,5
#include <bits/stdc++.h>
#include <omp.h>

#define ECHO 0

using namespace std;

void printm(double **m, unsigned n)
{
    unsigned i, j;
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            cout << fixed << setw(6) << setprecision(3) << setfill('0') << m[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

int main(int argc, char const *argv[])
{
    double **m, //matrix
           **t;
    unsigned   n, //matrix's size
               q, //number of iterations
               i,
               j,
               k;

    n = atoi(argv[1])+2;
    q = atoi(argv[2]);

    /*allocing memory for m*/
    m = new double*[n];
    #pragma omp parallel for
    for (i=0; i<n; i++) {
        m[i] = new double[n];
    }
    
    /*allocing memory for m*/
    t = new double*[n];
    #pragma omp parallel for
    for (i=0; i<n; i++) {
        t[i] = new double[n];
    }

    #pragma omp parallel for collapse(2)
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            if (i==0 || j==0 || i==n-1 || j==n-1) { /*Fill borders with 0's*/
                m[i][j]=0;
            } else {
                m[i][j]=i*j;
            }
        }
    }

    #pragma omp parallel for collapse(2)
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            if (i==0 || j==0 || i==n-1 || j==n-1) { /*Fill borders with 0's*/
                t[i][j]=0;
            }
        }
    }

    for (k=0; k<q; k++) {
        if (ECHO) printm(m, n);

        #pragma omp parallel for collapse(2)
        for (i=1; i<n-1; i++) {
            for (j=1; j<n-1; j++) {
                t[i][j]=(m[i-1][j]+m[i+1][j]+m[i][j-1]+m[i][j+1])*0.25;
            }
        }

        #pragma omp parallel for collapse(2)
        for (i=1; i<n-1; i++) {
            for (j=1; j<n-1; j++) {
                m[i][j]=t[i][j];
            }
        }
    }

    /*Freeing memory*/
    #pragma omp parallel for
    for (i=0; i<n; i++) {
        delete[] m[i];
    }
    delete[] m;

    /*Freeing memory*/
    #pragma omp parallel for
    for (i=0; i<n; i++) {
        delete[] t[i];
    }
    delete[] t;

    return 0;
}
