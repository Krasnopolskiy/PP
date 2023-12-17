#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


void insert_sort(int *array, int n, int stride) {
    for (int j = stride; j < n; j += stride) {
        int key = array[j];
        int i = j - stride;
        while (i >= 0 && array[i] > key) {
            array[i + stride] = array[i];
            i -= stride;
        }
        array[i + stride] = key;
    }
}

void shell_sort(int *array, int n) {
    int i, m;
    for (m = n / 2; m > 0; m /= 2) {
        for (i = 0; i < m; i++)
            insert_sort(&(array[i]), n - i, m);
    }
}

int main(int argc, char **argv) {
    const int count = 1000000;
    const int random_seed = 123123;
    const int iterations = 20;

    srand(random_seed);

    int *array = malloc(count * sizeof(int));

    double start_time, end_time, total = 0;

    for (int j = 0; j < iterations; j++) {
        for (int i = 0; i < count; i++) { array[i] = rand(); }
        start_time = omp_get_wtime();
        shell_sort(array, count);
        end_time = omp_get_wtime();
        total += end_time - start_time;
    }

    printf("Average time: %f\n", total / (double) iterations);

    free(array);
    return 0;
}