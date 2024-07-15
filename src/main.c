#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>


#define INPUT_FILENAME "programs.txt"

typedef struct {
    int id;
    char command[8];
    int * dependencies;
    int n_dependencies;
} Process;

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

int main(int argc, char *argv[]) {
    int status;
    int process_count;
    int tempo = 0;
    int count = 0;

    Process *processes;

    siginfo_t siginfo;

    if (argc != 2) {
        perror("Uso: escalona <numero_de_cores>");
        return 1;
    }
    const int num_cores = atoi(argv[1]);
    printf("Numero de cores: %d\n", num_cores);
  
    FILE *entrada = fopen(INPUT_FILENAME, "r");
    if (entrada == NULL) {
        fclose(entrada);
        perror("Erro ao abrir o arquivo de entrada!");
        return 1;
    }

    map_process(&processes, &process_count, entrada); 

    for(int i = 0; i < process_count; i++) {
        printf("%d, %s, ", processes[i].id, processes[i].command);
        for(int j = 0; j < processes[i].n_dependencies; j++) {
            printf("%d ", processes[i].dependencies[j]);
        }
        printf("\n");
    }

    fclose(entrada);

    // char buff[256];
    // int c = 0;
    // while (fgets(buff, sizeof(buff), entrada) != NULL) {
    //     printf("%s", buff);
    //     c++;
    // }

    // for(int i = 0; i < 3; i++) {
    //     pid_t pid = fork();
    //     if(pid==0){
    //         static char *argv[]={"teste15", NULL};
    //         execv("teste15.out", argv);
    //         printf("erro");
    //         exit(127);
    //     } 
    // }

    return 0;
}

