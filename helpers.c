#include "helpers.h"

void print_array(const char *name, int *array, int size) {
    printf("%s: [", name);
    for (int i = 0; i < size; i++) {
        printf("%d", array[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

void print_matrix(const char *name, int **matrix, int rows, int cols) {
    printf("%s:\n", name);
    for (int i = 0; i < rows; i++) {
        printf("Customer %d: [", i);
        for (int j = 0; j < cols; j++) {
            printf("%d", matrix[i][j]);
            if (j < cols - 1) printf(", ");
        }
        printf("]\n");
    }
}