#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <pthread.h>

#define NUM_THREADS 8

    int N = 512 * 1024 * 1024;
    int *A, *B;
    long dot_product = 0;
pthread_mutex_t mutex;

typedef struct {
    int start;
    int end;
} ThreadData;

void* dot_product_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    long long local_sum = 0;

    for (int i = data->start; i < data->end; i++) {
        local_sum += A[i] * B[i];
    }

    pthread_mutex_lock(&mutex);
    dot_product += local_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main () {
    double begin,end; // Timestamps for beginning and end of computation

    //Allocate Array A and B with N elements
    A = (int *) malloc (N * sizeof(int));
    B = (int *) malloc (N * sizeof(int));
    

    //Initialize arrays
    for (int i = 0; i < N; i++) {
        A[i] = 1;
        B[i] = 2;
    }
pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);

    int chunk = N / NUM_THREADS;

    //Now running time-stamp
    begin = omp_get_wtime();
    for (int i = 0; i < NUM_THREADS; i++) {
thread_data[i].start = i * chunk;
        thread_data[i].end = (i == NUM_THREADS - 1) ? N : (i + 1) * chunk;
        pthread_create(&threads[i], NULL, dot_product_thread, (void*)&thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    end = omp_get_wtime();

    printf("The final dotproduct is %ld\n", dot_product);
    printf("Time to execute was %lfs\n", end-begin);

    return 0;
}

