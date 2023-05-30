#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESS 5
#define MAX_RESOURCE 5

void readFiles(int max_resources[MAX_RESOURCE], int requests[MAX_PROCESS][MAX_RESOURCE], int allocation[MAX_PROCESS][MAX_RESOURCE]) {


    // Read max_resources resources file
    FILE* max_resources_file = fopen("resources.txt", "r");

    if (max_resources_file == NULL) {
        perror("Error opening file\n");
        exit(1);
    }
    int max_resources_i = 0;
    while(!feof(max_resources_file) && max_resources_i < MAX_RESOURCE){

        if(ferror(max_resources_file)) {
            perror("Error reading file\n");
            exit(1);
        }

        fscanf(max_resources_file, "%d", &max_resources[max_resources_i]);

        max_resources_i++;
    }

    fclose(max_resources_file);

    //Read allocations file
    FILE* alloc_file = fopen("allocations.txt", "r");

    if (alloc_file == NULL) {
        perror("Error opening file\n");
        exit(1);
    }

    int alloc_i = 0;
    while(!feof(alloc_file) && alloc_i < MAX_PROCESS){

        if(ferror(alloc_file)) {
            perror("Error reading file\n");
            exit(1);
        }

        for(int j = 0 ; j< MAX_RESOURCE; j++){

            fscanf(alloc_file, "%d", &allocation[alloc_i][j]);
        }

        alloc_i++;

    }

    fclose(alloc_file);

    //Read requests file
    FILE* requests_file = fopen("requests.txt", "r");

    if (requests_file == NULL) {
        perror("Error opening file\n");
        exit(1);
    }

    int requests_i = 0;

    while(!feof(requests_file) && requests_i < MAX_PROCESS){

        if(ferror(requests_file)) {
            perror("Error reading file\n");
            exit(1);
        }

        for(int j = 0 ; j< MAX_RESOURCE; j++){

            fscanf(requests_file, "%d", &requests[requests_i][j]);
        }

        requests_i++;

    }

    fclose(requests_file);

}

void printProcessesSummary(int allocation[MAX_PROCESS][MAX_RESOURCE], int need[MAX_PROCESS][MAX_RESOURCE]) {

    for(int process = 0; process < MAX_PROCESS; process++) {

        printf("\n");
        printf("Information for Process: P%d:\n", process + 1);

        printf("Allocated Resources: ");
        for(int resource = 0; resource < MAX_RESOURCE; resource++) {

            printf(" R%d: %d ", resource + 1, allocation[process][resource]);

        }

        printf("\nRequested Resources: ");
        for(int resource = 0; resource < MAX_RESOURCE; resource++) {

            printf(" R%d:%d ", resource + 1, need[process][resource]);

        }

        printf("\n");

    }

}



bool isSafeState(int available[MAX_RESOURCE], int allocation[MAX_PROCESS][MAX_RESOURCE], int need[MAX_PROCESS][MAX_RESOURCE], bool finish[MAX_PROCESS], int safe_sequence[MAX_PROCESS]) { 
    
    bool safe = true;

    int work[MAX_RESOURCE];
    for (int i = 0; i < MAX_RESOURCE; i++) {
        work[i] = available[i];
    }

    int seq_counter = 0;

    while(true) {
        bool found_process = false;

        for (int i = 0; i < MAX_PROCESS; i++) {

            if (finish[i]) continue;
            bool can_execute = true;

            for(int j = 0; j< MAX_RESOURCE; j++) {
                if (need[i][j] > work[j]) {
                    can_execute = false;
                    break;
                }
            }

            if(can_execute) {

                for(int j = 0; j< MAX_RESOURCE; j++) {
                    work[j] += allocation[i][j];
                }

                safe_sequence[seq_counter++] = i;
                finish[i] = true;
                found_process = true;

            }


        }

        if(!found_process) break;
    }

    for (int i = 0; i < MAX_PROCESS; i++) {
        if (!finish[i]) {
            safe = false;
            break;
        }
    }

    return safe;
}

void detectDeadlockOrSafe(int available[MAX_RESOURCE], int allocation[MAX_PROCESS][MAX_RESOURCE], int need[MAX_PROCESS][MAX_RESOURCE]) {

    bool finish[MAX_PROCESS] = { [0 ... MAX_PROCESS - 1] =  false };
    int safe_sequence[MAX_PROCESS] = { [0 ... MAX_PROCESS-1] = -1 };

    bool safe = isSafeState(available, allocation, need, finish, safe_sequence);

    printf("\n\nRESULTS: \n");

    printf("\nRunning order for processes: ");
    for (int i = 0; i < MAX_PROCESS; i++) {
        if(safe_sequence[i] != -1) printf("P%d ", safe_sequence[i] + 1);
    }

    printf("\n");

    if (safe) {
        printf("No deadlock detected.\n\n");

    } else {
        printf("Deadlock detected. Processes involved in deadlock: ");
        for (int i = 0; i < MAX_PROCESS; i++) {
            if (!finish[i]) {
                printf("P%d ", i + 1);
            }
        }
        printf("\n\n");
    }
}

int main() {
    int max_resources[MAX_RESOURCE];
    int max[MAX_PROCESS][MAX_RESOURCE];
    int allocation[MAX_PROCESS][MAX_RESOURCE];
    int need[MAX_PROCESS][MAX_RESOURCE];
    int safe_sequence[MAX_PROCESS];
    int available[MAX_RESOURCE];


    // Read input arrays from files
    readFiles(max_resources, need, allocation);

    // Calculate available resources
    for (int i = 0; i < MAX_RESOURCE; i++) {
        available[i] = max_resources[i];
        for (int j = 0; j < MAX_PROCESS; j++) {
            available[i] -= allocation[j][i];
        }
    }

    // Print the resource allocations, needs, and need vectors
    printProcessesSummary(allocation, need);


    detectDeadlockOrSafe(available, allocation, need);

    return 0;
}
