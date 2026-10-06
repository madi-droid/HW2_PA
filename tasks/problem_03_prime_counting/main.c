#include <stdio.h>
#include <omp.h>

int is_prime(int n) {
    if (n < 2) {
        return 0;
    }
    // n / i avoids overflow in the condition i * i <= n.
    for (int i = 2; i <= n / i; i++) {
        if (n % i == 0) {
            return 0;
        }
    }

    return 1;
}

int main(void) {
    int N;
    printf("Enter N: ");
    if (scanf("%d", &N) != 1 || N < 2) {
        printf("N must be at least 2\n");
        return 1;
    }

    int count = 0;
    double start = omp_get_wtime();

#pragma omp parallel for
    for (long long i = 2; i <= N; i++) {
        if (is_prime((int)i)) {
#pragma omp atomic
            count++;
        }
    }

    double end = omp_get_wtime();

    printf("Number of primes: %d\n", count);
    printf("Time: %f\n", end - start);

    return 0;
}
