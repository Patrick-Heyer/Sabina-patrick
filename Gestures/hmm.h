#include "observations.h"

class hmm {
private:
    // Members
    double **A; //Matrix of transitions
    double **B; //Matrix of emission
    double *pi; //Array of initial states
    int N; //Number of states
    int M; //Number of symbols of outs

    //Privates functions
    double **allmemory2d(int height, int width);
    double *allmemory1d(int width);
    void deallmemory2d(double ** M,int height);
    void deallmemory1d(double * M);

public:
    // Constructor and destructor

    hmm(char a[], char b[], int n, int m);
    ~hmm() {
        deallmemory2d(A, N);
        deallmemory2d(B, N);
        deallmemory1d(pi);
    }

    // Functions members
    double ForwardWithScale(int T, NODO *O);
};
