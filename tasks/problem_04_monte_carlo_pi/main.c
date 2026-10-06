#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void) {
    int N;
    printf("Enter number of points: ");
    if (scanf("%d", &N) != 1 || N <= 0) {
        printf("N must be positive\n");
        return 1;
    }

    double *X = malloc((size_t)N * sizeof(double));
    double *Y = malloc((size_t)N * sizeof(double));
    int threads = omp_get_max_threads();
    int *counts = calloc(threads, sizeof(int));

    if (X == NULL || Y == NULL || counts == NULL) {
        printf("Memory allocation failed\n");
        free(X);
        free(Y);
        free(counts);
        return 1;
    }

    int total = 0;
    double pi = 0.0;
    double start = omp_get_wtime();

    // Generate points before the parallel region: rand() has shared state.
    srand(1);
    for (int i = 0; i < N; i++) {
        X[i] = (double)rand() / (RAND_MAX + 1.0);
        Y[i] = (double)rand() / (RAND_MAX + 1.0);
    }

#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int local_count = 0;

#pragma omp for nowait
        for (int i = 0; i < N; i++) {
            if (X[i] * X[i] + Y[i] * Y[i] <= 1.0) {
                local_count++;
            }
        }

        counts[id] = local_count;

        // Wait until every thread has stored its count.
#pragma omp barrier
#pragma omp single
        {
            for (int i = 0; i < omp_get_num_threads(); i++) {
                total += counts[i];
            }
            pi = 4.0 * total / N;
        }
    }

    double end = omp_get_wtime();

    printf("Points: %d\n", N);
    printf("Points inside circle: %d\n", total);
    printf("Pi: %.10f\n", pi);
    printf("Time: %f\n", end - start);

    free(X);
    free(Y);
    free(counts);
    return 0;
}
