#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int check_prime(int n) {
    if (n < 2) {
        return 0;
    }

    int bound = (int) sqrt(n);
    for (int i = 2; i <= bound; i++) {
        if (n % i == 0) return 0;
    }

    return 1;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <start> <end>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int start = atoi(argv[1]);
    int end = atoi(argv[2]);

    int primes = 0;
    double start_time, end_time;

    start_time = omp_get_wtime();

    for (int n = start; n <= end; n++) {
        primes += check_prime(n);
    }

    end_time = omp_get_wtime();

    printf("Prime numbers between %d and %d: %d\n", start, end, primes);
    printf("Execution time: %f\n", end_time - start_time);

    return 0;
}
