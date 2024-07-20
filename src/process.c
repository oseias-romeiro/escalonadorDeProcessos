#include "process.h"

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
    }
} 