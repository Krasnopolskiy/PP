#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv) {
    const int count = 10000000;
    const int random_seed = 1337;
    const int attempts = 25;
    const int target = 1337;

    srand(random_seed);

    int index;
    int *array = malloc(count * sizeof(int));
    for (int i = 0; i < count; i++) { array[i] = rand(); }

    double start_time, end_time, total = 0;

    for (int j = 0; j < attempts; j++) {
        index = -1;
        start_time = omp_get_wtime();
        for (int i = 0; i < count; i++) {
            if (array[i] == target) {
                index = i;
                break;
            }
        }
        end_time = omp_get_wtime();
        total += end_time - start_time;
    }
    printf("Average time: %f\n", total / (double) attempts);

    free(array);
    return 0;
}
