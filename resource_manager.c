#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "resource_manager.h"

extern int num_customers;
extern int num_resources;
extern int **maximum;
extern int *available;
extern int **allocation;
extern int **need;

int request_resources(int customer_num, int request[]) {

    // Step 1: Check if request is less than or equal to 'need' and 'available'
    for (int i = 0; i < num_resources; i++) {
        if (request[i] > need[customer_num][i]) {
            fprintf(stderr, "Error: Request exceeds customer's need for resource %d.\n", i);
            return -1;
        }
        if (request[i] > available[i]) {
            fprintf(stderr, "Error: Request exceeds available resources for resource %d.\n", i);
            return -1;
        }
    }

    // Step 2: Tentatively allocate the resources
    int *work = (int *)malloc(num_resources * sizeof(int));
    bool *finish = (bool *)malloc(num_customers * sizeof(bool));    // Bool array to check if there's a safe sequence
    int **temp_allocation = (int **)malloc(num_customers * sizeof(int *));
    int **temp_need = (int **)malloc(num_customers * sizeof(int *));

    memcpy(work, available, num_resources * sizeof(int));

    for (int i = 0; i < num_customers; i++) {
        finish[i] = false;
    }

    for (int i = 0; i < num_customers; i++) {
        temp_allocation[i] = (int *)malloc(num_resources * sizeof(int));
        temp_need[i] = (int *)malloc(num_resources * sizeof(int));
        memcpy(temp_allocation[i], allocation[i], num_resources * sizeof(int));
        memcpy(temp_need[i], need[i], num_resources * sizeof(int));
    }

    for (int i = 0; i < num_resources; i++) {
        work[i] -= request[i];
        temp_allocation[customer_num][i] += request[i];
        temp_need[customer_num][i] -= request[i];
    }

    // Step 3: Perform Safety Check
    while (true) {
        bool progress_made = false;
        for (int i = 0; i < num_customers; i++) {
            if (!finish[i]) {
                bool can_finish = true;
                for (int j = 0; j < num_resources; j++) {
                    if (temp_need[i][j] > work[j]) {    // If needed resources are greater than what's available, the customer's request cannot be granted
                        can_finish = false;
                        break;
                    }
                }
                if (can_finish) {   // If this is met, the customer's allocation request can be granted
                    for (int j = 0; j < num_resources; j++) work[j] += temp_allocation[i][j];
                    finish[i] = true;
                    progress_made = true;
                }
            }
        }
        if (!progress_made) break;  // Exhausted customer requests or no safe state because a customer can't finish allocation
    }

    // Check if all customers can finish their requests
    bool safe = true;
    for (int i = 0; i < num_customers; i++) {
        if (!finish[i]) {
            safe = false;
            break;
        }
    }

    // Step 4: Decide to grant or deny the Request
    if (safe) {
        for (int i = 0; i < num_resources; i++) {
            available[i] -= request[i];
            allocation[customer_num][i] += request[i];
            need[customer_num][i] -= request[i];
        }
    }

    free(work);
    free(finish);
    for (int i = 0; i < num_customers; i++) {
        free(temp_allocation[i]);
        free(temp_need[i]);
    }
    free(temp_allocation);
    free(temp_need);

    if (safe) return 0;
    else return -1;
}


void release_resources(int customer_num, int release[]) {
    
    // Check if release exceeds allocation or there's negative value provided
    for (int i = 0; i < num_resources; i++) {
        if (release[i] > allocation[customer_num][i]) {
            fprintf(stderr, "Error: Cannot release more resources than allocated for resource %d.\n", i);
            fprintf(stderr, "Allocated: %d, Attempted to release: %d\n", allocation[customer_num][i], release[i]);
            return;
        }
        if (release[i] < 0) {
            fprintf(stderr, "Error: Release amount cannot be negative for resource %d.\n", i);
            return;
        }
    }

    for (int i = 0; i < num_resources; i++) {
        available[i] += release[i];
        allocation[customer_num][i] -= release[i];
        need[customer_num][i] += release[i];
    }

    printf("Resources released successfully.\n");
}
