#include <stdio.h>
#include <omp.h>

long long fib_seq(int n) {
    if (n <= 1) {
        return n;
    }

    long long a = 0;
    long long b = 1;

    for (int i = 2; i <= n; i++) {
        long long c = a + b;
        a = b;
        b = c;
    }

    return b;
}

long long fib(int n) {
    // Small values are computed without creating tasks.
    if (n <= 20) {
        return fib_seq(n);
    }

    long long a;
    long long b;

#pragma omp task shared(a) firstprivate(n)
    {
        a = fib(n - 1);
    }

#pragma omp task shared(b) firstprivate(n)
    {
        b = fib(n - 2);
    }

#pragma omp taskwait
    return a + b;
}

int main(void) {
    int n;
    printf("Enter n (0-40): ");
    // Larger n creates too many recursive tasks for this example.
    if (scanf("%d", &n) != 1 || n < 0 || n > 40) {
        printf("n must be between 0 and 40\n");
        return 1;
    }

    long long result = 0;
    double start = omp_get_wtime();

#pragma omp parallel
    {
#pragma omp single
        {
            result = fib(n);
        }
    }

    double end = omp_get_wtime();
    printf("F(%d): %lld\n", n, result);
    printf("Time: %f\n", end - start);

    return 0;
}
