/*
Simulation of the Banker's Algorithm for OS resource management

Program execution:
    1. Read the txt file provided. NOTE: Filename is defined below, must match actual file name
    2. Dynamically allocate total number of customers and resources from given txt file, as well as all arrays needed
    3. Populate the 'maximum' and 'need' array with txt file values
    4. Check whether available resources passed from command-line match total number from txt file
    5. Populate 'available' array accordingly
    6. Prompt user for valid command
    7. Either try allocating requested resources, release them, print system state, or quit program
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "file_reader.h"
#include "resource_manager.h"
#include "helpers.h"
#define FILE_NAME "customer_requests.txt"


int main(int argc, char** argv) {

    printf("Welcome to the Banker's Algorithm Simulation!\n");

    printf("\nReading maximum requests file:\n");

    if (read_file(FILE_NAME) == 0) {

        printf("\nTotal customers: %d\n", num_customers);
        printf("Total resources: %d\n", num_resources);

        printf("\nMaximum requests initialized:\n");
        for (int i = 0; i < num_customers; i++) {
            printf("Customer %d: [ ", i);
            for (int j = 0; j < num_resources; j++) {
                printf("%d ", maximum[i][j]);
            }
            printf("]\n");
        }

    } else {
        fprintf(stderr, "Failed to read the file.\n");
        return 1;
    }

    // Check cla's and see if they match the number of total resources from the txt file!
    if ((argc - 1) != num_resources) {
        fprintf(stderr, "\nError: Number of available resources do not match number of resources from txt file!\n");
        fprintf(stderr, "Expected %d command-line arguments; got %d\n", num_resources, (argc - 1));
        return 1;
    }

    for (int i = 0; i < num_resources; i++)
        available[i] = atoi(argv[i+1]);

    printf("\nInitializing system with available resources:\n");
    printf("Available: [ ");
    for (int j = 0; j < num_resources; j++)
        printf("%d ", available[j]);
    printf("]\n");

    char line[1024];
    char *command;
    bool quit_program = false;

    while (!quit_program) {

        printf("\nPlease enter commands: \n");
        printf("- 'RQ customer_num resource_1 resource_2 resource_3 ... resource_n' to request resources\n");
        printf("- 'RL customer_num resource_1 resource_2 resource_3 ... resource_n' to release resources\n");
        printf("- '*' to display the current values of the data structures\n");
        printf("- 'exit' to quit\n\n");

        if (fgets(line, sizeof(line), stdin) != NULL) {

            line[strcspn(line, "\n")] = '\0';
            command = strtok(line, " ");

            if (command != NULL) {

                char *rest_of_line = strtok(NULL, "");
                if (rest_of_line == NULL) rest_of_line = "";

                if (strcmp(command, "RQ") == 0) {
                    printf("Processing RQ command...\n");
                    if (strlen(rest_of_line) == 0) {
                        fprintf(stderr, "Error: No arguments provided for RQ command. Please provide customer number and requested resources.\n");
                        continue;
                    }
                    char *arg = strtok(rest_of_line, " ");
                    char *endptr;
                    int customer_num = strtol(arg, &endptr, 10);    // strtol converts string to integer; last arg is int base
                    if (*endptr != '\0') {
                        fprintf(stderr, "Error: Invalid customer number '%s'.\n", arg);
                        continue;
                    }
                    if (customer_num < 0 || customer_num >= num_customers) {
                        fprintf(stderr, "Error: Invalid customer number %d. Must be between 0 and %d.\n", customer_num, num_customers - 1);
                        continue;
                    }

                    int request[num_resources];
                    int i = 0;
                    while ((arg = strtok(NULL, " ")) != NULL) {
                        int value = strtol(arg, &endptr, 10);
                        if (*endptr != '\0') {
                            fprintf(stderr, "Error: Invalid integer value '%s' in 'request' array.\n", arg);
                            break;
                        }
                        request[i++] = value;
                    }

                    if (i != num_resources) {
                        fprintf(stderr, "Error: Incorrect number of resource values provided. Expected %d, got %d.\n", num_resources, i);
                        continue;
                    }

                    int result = request_resources(customer_num, request);
                    if (result == 0) {
                        printf("Request granted. System is in a safe state.\n");
                    } else {
                        printf("Request denied. System would be left in an unsafe state.\n");
                    }

                } else if (strcmp(command, "RL") == 0) {
                    printf("Processing RL command...\n");

                    if (strlen(rest_of_line) == 0) {
                        fprintf(stderr, "Error: No arguments provided for RL command. Please provide customer number and resources to release.\n");
                        continue;
                    }

                    char *arg = strtok(rest_of_line, " ");
                    char *endptr;
                    int customer_num = strtol(arg, &endptr, 10);
                    if (*endptr != '\0') {
                        fprintf(stderr, "Error: Invalid customer number '%s'.\n", arg);
                        continue;
                    }
                    if (customer_num < 0 || customer_num >= num_customers) {
                        fprintf(stderr, "Error: Invalid customer number %d. Must be between 0 and %d.\n", customer_num, num_customers - 1);
                        continue;
                    }

                    int release[num_resources];
                    int i = 0;
                    while ((arg = strtok(NULL, " ")) != NULL) {
                        int value = strtol(arg, &endptr, 10);
                        if (*endptr != '\0') {
                            fprintf(stderr, "Error: Invalid integer value '%s' in 'release' array.\n", arg);
                            break;
                        }
                        release[i++] = value;
                    }
                    if (i != num_resources) {
                        fprintf(stderr, "Error: Incorrect number of resource values provided. Expected %d, got %d.\n", num_resources, i);
                        continue;
                    }

                    release_resources(customer_num, release);

                } else if (strcmp(command, "*") == 0) {
                    printf("Processing * command...\n");
                    printf("Current system state:\n");

                    print_array("Available", available, num_resources);
                    print_matrix("Maximum", maximum, num_customers, num_resources);
                    print_matrix("Allocation", allocation, num_customers, num_resources);
                    print_matrix("Need", need, num_customers, num_resources);

                } else if (strcmp(command, "exit") == 0) {
                    printf("Exiting program. Goodbye!\n");
                    quit_program = true;

                } else {
                    fprintf(stderr, "Unknown command: %s\n", command);
                    fprintf(stderr, "Please enter a valid command!\n");
                }
            } else {
                fprintf(stderr, "No command detected; enter a valid command!\n");
            }
        } else {
            fprintf(stderr, "Error reading input. Goodbye!\n");
            quit_program = true;
        }
    }

    for (int i = 0; i < num_customers; i++) {
        free(maximum[i]);
        free(need[i]);
        free(allocation[i]);
    }
    free(maximum);
    free(need);
    free(allocation);
    free(available);

    return 0;
}