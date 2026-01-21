#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <pthread.h>

int main () {
    int N = 512 * 1024 * 1024;
    int *A, *B;
    long dot_product = 0;
    double begin,end; // Timestamps for beginning and end of computation

    //Allocate Array A and B with N elements
    A = (int *) malloc (N * sizeof(int));
    B = (int *) malloc (N * sizeof(int));
    

    //Initialize arrays
    for (int i = 0; i < N; i++) {
        A[i] = 1;
        B[i] = 1;
    }

    //Now running time-stamp
    begin = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        dot_product += A[i] * B[i];
    }
    end = omp_get_wtime();

    printf("The final dotproduct is %ld\n", dot_product);
    printf("Time to execute was %lfs\n", end-begin);

    return 0;
}

