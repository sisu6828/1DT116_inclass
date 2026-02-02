#include <stdio.h>
#include <omp.h>
#include <math.h>

double step;
static long num_steps = 100000000L;
static long num_steps_monte_carlo = 100000000L;

int main (){
    double x, pi = 0.0, sum=0.0;
    step = 1.0/(double) num_steps; // step number of very small bars!
    double T1, T2;

    T1 = omp_get_wtime();

    for (long i = 0; i < num_steps; i++) {
        x = step * i;
        sum += step * 4.0 / (1 + x*x);
    }

    T2 = omp_get_wtime();
    pi = sum;

    printf("Time: %lf, pi: %lf\n", T2-T1, pi);

    long count_red = 0, count_blue = 0;

    unsigned *seeds = malloc(omp_get_num_threads() * 128);

    T1 = omp_get_wtime();
#pragma omp parallel for reduction(+:count_red, count_blue)
    for (long i = 0; i < num_steps_monte_carlo; i++) {
        unsigned int tid = omp_get_thread_num();
        unsigned *seed = &seeds[tid*32];
        double x = (double)rand_r(seed) / (double)RAND_MAX;
        double y = (double)rand_r(seed) / (double)RAND_MAX;
        double distance = sqrt(x*x + y*y);


        if (distance <= (double)1.0)
            count_red++;
        else
            count_blue++;
    }

    pi = (double)count_red / (double)(count_red+count_blue) * 4;
    T2 = omp_get_wtime();

    printf("Time: %lf, pi: %lf\n", T2-T1, pi);


    return 0;
}
