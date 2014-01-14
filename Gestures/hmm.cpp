#include "hmm.h"
#include <math.h>
#include <iostream>
#include <fstream>

using namespace std;

hmm::hmm(char a[], char b[], int n, int m) {

    A = allmemory2d(n, n);
    B = allmemory2d(n, m);
    pi = allmemory1d(n);
    N = n;
    M = m;
    int i, j;

    ifstream fe(a);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            fe >> A[i][j];
        }
    }
    fe.close();
    fe.open(b);
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            fe >> B[i][j];
        }
    }
    fe.close();

    pi[0] = 1;
    pi[1] = 0;
    pi[2] = 0;
}

double ** hmm::allmemory2d(int height, int width) {
    double **M;

    M = new double*[height];
    for (int i = 0; i < height; ++i)
        M[i] = new double[width];
    return M;
}

double * hmm::allmemory1d(int width) {

    double *M = new double[width];;

    return M;
}

void hmm::deallmemory2d(double ** M, int height) {
    for (int i = 0; i < height; ++i)
        delete [] M[i];
    delete [] M;
}

void hmm::deallmemory1d(double * M) {

    delete [] M;
}

double hmm::ForwardWithScale(int T, NODO *O)
/*  pprob is the LOG probability */
{
    int	i, j; 	/* state indices */
    int	t;	/* time index */
    double **alpha = allmemory2d(T+1, N);
    double *scale  = allmemory1d(T+1);
    double pprob;
    double sum;	/* partial sum */

    /* 1. Initialization */

    scale[0] = 0.0;
    for (i = 0; i < N; i++) {
        alpha[0][i] = pi[i]* (B[i][O->data]);
        scale[0] += alpha[0][i];
    }
    for (i = 0; i < N; i++)
        alpha[0][i] /= scale[0];

    /* 2. Induction */

    for (t = 0; t < T-1; t++) {
        scale[t+1] = 0.0;
        O=O->next;
        for (j = 0; j < N; j++) {
            sum = 0.0;
            for (i = 0; i < N; i++)
                sum += alpha[t][i]* (A[i][j]);

            alpha[t+1][j] = sum*(B[j][O->data]);
            scale[t+1] += alpha[t+1][j];
        }
        for (j = 0; j < N; j++)
            alpha[t+1][j] /= scale[t+1];
        cout << "entro" << endl;
    }

    /* 3. Termination */
    pprob = 0.0;

    for (t = 0; t < T; t++)
        pprob += log(scale[t]);

    deallmemory2d(alpha, T+1);
    deallmemory1d(scale);


    if (pprob <0 ) pprob=pprob*-1;
    cout << "prob " << pprob << endl;
    return pprob;
}
