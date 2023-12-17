#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <mpi.h>

void check_prime(int n, int *primes) {
    int prime = 1, bound = (int) sqrt(n);

    if (n < 2) {
        prime = 0;
    }

#pragma omp parallel shared(prime, bound)
    {
#pragma omp for
        for (int i = 2; i <= bound; i++) {
            if (n % i == 0) prime = 0;
            if (prime == 0) i = bound + 1;
        }
    }

    if (prime) {
#pragma omp critical
        {
            *primes += 1;
        }
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc != 3) {
        if (rank == 0) {
            fprintf(stderr, "Usage: %s <start> <end>\n", argv[0]);
        }
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    int start = atoi(argv[1]);
    int end = atoi(argv[2]);

    int chunk = (end - start + 1) / size;
    int rem = (end - start + 1) % size;

    int local_start = start + rank * chunk;
    int local_end = local_start + chunk - 1;

    if (rank < rem) {
        local_start += rank;
        local_end += 1;
    } else {
        local_start += rem;
    }

    int result = 0;
    double start_time, end_time;

    int max_threads = omp_get_max_threads();
    int threads = (size > max_threads) ? max_threads / size : 2;
    omp_set_num_threads(threads);

    MPI_Barrier(MPI_COMM_WORLD);

    start_time = MPI_Wtime();

#pragma omp parallel for
    for (int n = local_start; n <= local_end; n++) {
        check_prime(n, &result);
    }

    end_time = MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD);

    double benchmark = end_time - start_time;

    double *benchmarks;
    if (rank == 0) {
        benchmarks = (double *) malloc(size * sizeof(double));
    }

    MPI_Gather(&benchmark, 1, MPI_DOUBLE, benchmarks, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int* results;
    if (rank == 0) {
        results = (int*)malloc(size * sizeof(int));
    }

    MPI_Gather(&result, 1, MPI_INT, results, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Process\tTime\tPrimes\n");
        for (int i = 0; i < size; i++) {
            printf("%d\t%f\t%d\n", i, benchmarks[i], results[i]);
        }

        int total_result = 0;
        for (int i = 0; i < size; i++) {
            total_result += results[i];
        }

        printf("Total: %d\n", total_result);

        free(benchmarks);
        free(results);
    }

    MPI_Finalize();

    return 0;
}
