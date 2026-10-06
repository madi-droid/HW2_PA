#include <math.h>
#include <stdio.h>
#include <omp.h>

int main(void) {
    int N;
    printf("Enter number of intervals: ");
    if (scanf("%d", &N) != 1 || N <= 0) {
        printf("N must be positive\n");
        return 1;
    }

    double step = 1.0 / N;
    double sum = 0.0;
    double start = omp_get_wtime();

#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < N; i++) {
        // Use the middle of each interval.
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    double pi = sum * step;
    double end = omp_get_wtime();

    printf("Pi: %.15f\n", pi);
    printf("Error: %.15e\n", fabs(pi - acos(-1.0)));
    printf("Time: %f\n", end - start);

    return 0;
}
