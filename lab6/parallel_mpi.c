#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

void insert_sort(int *array, int n, int gap) {
    for (int j = gap; j < n; j += gap) {
        int tmp = array[j];
        int i = j - gap;
        while (i >= 0 && array[i] > tmp) {
            array[i + gap] = array[i];
            i -= gap;
        }
        array[i + gap] = tmp;
    }
}

void shell_sort(int *array, int n) {
    int i, gap;
    for (gap = n / 2; gap > 0; gap /= 2) {
        for (i = 0; i < gap; i++)
            insert_sort(&(array[i]), n - i, gap);
    }
}

int main(int argc, char **argv) {
    const int count = 1000000;
    const int random_seed = 1337;
    const int iterations = 25;

    srand(random_seed);

    int *array = malloc(count * sizeof(int));

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int chunk = count / size;

    double start_time, end_time, total = 0;
    for (int iteration = 0; iteration < iterations; iteration++) {
        int *larray, lsize = count - chunk * (size - 1);
        MPI_Status status;

        if (rank == 0) {
            for (int i = 0; i < count; i++) { array[i] = rand(); }

            for (int dest = 1; dest < size; dest++) {
                MPI_Send(array + chunk * (dest - 1), chunk, MPI_INT, dest, 0, MPI_COMM_WORLD);
            }

            start_time = MPI_Wtime();

            larray = malloc(lsize * sizeof(int));

            for (int i = chunk * (size - 1), j = 0; i < count; i++, j++) {
                larray[j] = array[i];
            }

            shell_sort(larray, lsize);

            int *sorted = malloc(count * sizeof(int));

            memcpy(sorted, larray, lsize * sizeof(int));

            for (int src = 1; src < size; src++) {
                MPI_Recv(sorted + chunk * src, lsize, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
            }

            shell_sort(sorted, count);

            free(sorted);
            free(larray);

            end_time = MPI_Wtime();
        } else {
            larray = malloc(chunk * sizeof(int));

            MPI_Recv(larray, chunk, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

            shell_sort(larray, chunk);

            MPI_Send(larray, chunk, MPI_INT, 0, 1, MPI_COMM_WORLD);

            free(larray);
        }

        total += end_time - start_time;
    }

    if (rank == 0) {
        printf("%d\t%f\n", size, total / (double) iterations);
    }

    MPI_Finalize();

    free(array);
    return 0;
}
