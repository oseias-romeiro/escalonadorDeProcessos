#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "process.h"

#define MAX_INT 1000000

typedef struct process {
    int id;
    char command[8];
    int * dependencies;
    int n_dependencies;
    char status;
    int own_dependencies_time;
} Process;

// Status list
// W - Waiting dependencies
// R - Ready
// F - Finished
// E - Executing

void map_process(Process** processes, int * process_count, FILE *file){
    int ch, count = 0;
    while((ch = fgetc(file)) != EOF){
        if(ch == '\n'){
            count++;
        }
    }

    *process_count = count;

    rewind(file);

    *processes = (Process*)malloc(count * sizeof(Process));
    if (*processes == NULL) {
        perror("Failed to allocate memory for processes");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++) {
        int id, dep_count = 0;
        char command[8];
        char dependencies_str[10000];
        int* dependencies = NULL;

        if (fscanf(file, "%d %s %s\n", &id, command, dependencies_str) != 3) {
            perror("Failed to read a line from the file");
            exit(EXIT_FAILURE);
        }

        char* token = strtok(dependencies_str, ",");
        while (strcmp(token, "#") != 0) {
            dependencies = (int*)realloc(dependencies, (dep_count + 1) * sizeof(int));
            if (dependencies == NULL) {
                perror("Failed to allocate memory for dependencies");
                exit(EXIT_FAILURE);
            }
            dependencies[dep_count++] = atoi(token);
            token = strtok(NULL, ",");
        }

        (*processes)[i].id = id;
        (*processes)[i].n_dependencies = dep_count;
        strncpy((*processes)[i].command, command, sizeof((*processes)[i].command) - 1);
        (*processes)[i].command[sizeof((*processes)[i].command) - 1] = '\0'; // Ensure null-termination
        (*processes)[i].dependencies = dependencies;
        if (id == 1) {
            (*processes)[i].status = 'R';
        } else {
            (*processes)[i].status = 'W';
        }
    }
} 

void calculate_own_dependencies(Process *processes, int process_count) {
    for (int i = 0; i < process_count; i++) {
        int own_dependencies_time = 0;
        for (int j = 0; j < process_count; j++) {
            for (int k = 0; k < processes[j].n_dependencies; k++) {
                if (processes[j].dependencies[k] == processes[i].id) {
                    own_dependencies_time++;
                }
            }
        }
        processes[i].own_dependencies_time = own_dependencies_time;
    }
}

void update_dependencies(Process *processes, int process_count, int id) {
    for (int i = 0; i < process_count; i++) {
        if (processes[i].status == 'F') {
            continue;
        }
        int update_status = 1;
        for (int j = 0; j < processes[i].n_dependencies; j++) {
            if (processes[i].dependencies[j] == id) {
                processes[i].dependencies[j] = 0;
            }
            if (processes[i].dependencies[j] != 0) {
                update_status = 0;
            }
        }
        if (update_status) {
            processes[i].status = 'R';
        }
    }
}

void finish_process(Process *processes, int process_count, int id) {
    for (int i = 0; i < process_count; i++) {
        if (processes[i].id == id) {
            processes[i].status = 'F';
            // printf("Processo %d finalizado\n", id);
            break;
        }
    }
    update_dependencies(processes, process_count, id);
}

int find_next_process(Process *processes, int process_count) {
    int np_index = -1;
    int np_time = MAX_INT;
    int np_own_dependencies = MAX_INT;
    for (int i = 0; i < process_count; i++) {
        if (processes[i].status == 'R') {
            int time = atoi(processes[i].command + 5);
            if (time < np_time) {
                if (processes[i].own_dependencies_time < np_own_dependencies) {
                    np_time = time;
                    np_index = i;
                }
            }
        }
    }
    return np_index;
}

void run_on_single_core(Process *processes, int process_count) {
    int status;
    int process_count_finished = 0;
    while (process_count_finished < process_count) {
        int next_process_index = find_next_process(processes, process_count);
        if (next_process_index == -1) {
            continue;
        }
        Process *next_process = &processes[next_process_index];
        pid_t pid = fork();
        if (pid == 0) {
            printf("Executando processo %d\n", next_process->id);
            sleep(atoi(next_process->command + 5));
            printf("Processo %d finalizado\n", next_process->id);
            exit(0);
        } else {
            waitpid(pid, &status, 0);
            finish_process(processes, process_count, next_process->id);
            process_count_finished++;
        }
    }
}

void print_processes(Process *processes, int process_count) {
    for (int i = 0; i < process_count; i++) {
        printf("%d, %s, ", processes[i].id, processes[i].command);
        for (int j = 0; j < processes[i].n_dependencies; j++) {
            printf("%d ", processes[i].dependencies[j]);
        }
        printf("\n");
    }
}