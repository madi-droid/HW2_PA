#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void) {
    int N;
    scanf("%d", &N);

    if (N<1000000) {
        printf("N must be at least 1000000\n");
        return 1;
    }

    int *A = malloc(N * sizeof(int));
    int *B = malloc(N * sizeof(int));
    int *C_par = malloc(N * sizeof(int));
    int *C_seq = malloc(N * sizeof(int));

    if (A == NULL || B == NULL || C_par == NULL || C_seq == NULL) {
        printf("Error\n");
        free(A);
        free(B);
        free(C_par);
        free(C_seq);
        return 1;
    }

    for (int i=0; i<N; i++) {
        A[i] = i;
        B[i] = i*2;
    }

    double start_par = omp_get_wtime();
#pragma omp parallel for
    for (int i =  0; i<N; i++) {
        C_par[i] = A[i]+B[i];
    }
    double end_par = omp_get_wtime();

    printf("\nFirst 5 elements:\n");
    for (int i = 0; i < 5; i++) {
        printf("C[%d] = %d\n", i, C_par[i]);
    }

    printf("\nLast 5 elements:\n");
    for (int i = N - 5; i < N; i++) {
        printf("C[%d] = %d\n", i, C_par[i]);
    }

    // ----------------------------------------

    double start_seq = omp_get_wtime();
    for (int i =  0; i<N; i++) {
        C_seq[i] = A[i] + B[i];
    }
    double end_seq = omp_get_wtime();

    double parallel_time = end_par - start_par;
    double sequential_time = end_seq - start_seq;

    printf("\nParallel time: %.9f seconds\n", parallel_time);
    printf("Sequential time: %.9f seconds\n", sequential_time);

    if (parallel_time > 0.0 && sequential_time > 0.0) {
        printf("Speedup: %f\n", sequential_time / parallel_time);
    } else {
        printf("Time is too small to measure speedup reliably\n");
    }

    free(A);
    free(B);
    free(C_par);
    free(C_seq);

    return 0;
}
