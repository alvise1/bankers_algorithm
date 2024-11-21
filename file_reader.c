#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_reader.h"

// Global variables
int num_customers = 0;
int num_resources = 0;
int **maximum = NULL;
int *available = NULL;
int **allocation = NULL;
int **need = NULL;

int read_file(const char *filename) {
    
    FILE *file;
    char line[1024];

    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        return -1;
    }

    while (fgets(line, sizeof(line), file)) {
        num_customers++;
        int current_num_resources = 0;

        char *temp_line = strdup(line);
        char *token = strtok(temp_line, ",\n");
        while (token != NULL) {
            current_num_resources++;
            token = strtok(NULL, ",\n");
        }
        free(temp_line);

        if (num_customers == 1) {
            num_resources = current_num_resources;
        } else {
            // For subsequent lines, we check if the number of resources matches expected value
            if (current_num_resources != num_resources) {
                fprintf(stderr, "\nError: Inconsistent number of resources in line %d. Expected %d, got %d.\n", num_customers, num_resources, current_num_resources);
                fclose(file);
                return -1;
            }
        }
    }

    fclose(file);

    if (num_customers == 0 || num_resources == 0) {
        fprintf(stderr, "File is empty or improperly formatted\n");
        return -1;
    }

    maximum = (int **)malloc(num_customers * sizeof(int *));
    need = (int **)malloc(num_customers * sizeof(int *));
    allocation = (int **)malloc(num_customers * sizeof(int *));
    available = (int *)malloc(num_resources * sizeof(int));

    if (maximum == NULL || need == NULL || allocation == NULL || available == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }

    for (int i = 0; i < num_customers; i++) {
        maximum[i] = (int *)malloc(num_resources * sizeof(int));
        need[i] = (int *)malloc(num_resources * sizeof(int));
        allocation[i] = (int *)malloc(num_resources * sizeof(int));

        if (maximum[i] == NULL || need[i] == NULL || allocation[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return -1;
        }
        for (int j = 0; j < num_resources; j++) allocation[i][j] = 0;   // Initially set values to 0
    }

    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        return -1;
    }

    int row = 0;
    while (fgets(line, sizeof(line), file) && row < num_customers) {
        // Remove the newline character at the end if present
        line[strcspn(line, "\n")] = '\0';

        char *token = strtok(line, ",");
        int col = 0;
        while (token != NULL && col < num_resources) {
            int value = atoi(token);
            maximum[row][col] = value;
            need[row][col] = value;     // Initially, 'need' array is the same as 'maximum'
            token = strtok(NULL, ",");
            col++;
        }
        row++;
    }

    fclose(file);

    return 0;
}