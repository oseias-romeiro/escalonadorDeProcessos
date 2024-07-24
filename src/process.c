#include "process.h"

#define MAX_INT 1000000

typedef struct process {
    int id;
    char command[8];
    int * dependencies;
    int n_dependencies;
    char status;
    int own_dependencies;
    pid_t pid;
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
            if(strcmp(token, "0") != 0){
                dependencies[dep_count++] = atoi(token);
            }
            token = strtok(NULL, ",");
        }

        (*processes)[i].id = id;
        (*processes)[i].n_dependencies = dep_count;
        strncpy((*processes)[i].command, command, sizeof((*processes)[i].command) - 1);
        (*processes)[i].command[sizeof((*processes)[i].command) - 1] = '\0'; // Ensure null-termination
        (*processes)[i].dependencies = dependencies;
        // coloca em Ready se não tiver dependencias
        if(dep_count == 0){
            printf("Processo %d pronto\n", id);
            (*processes)[i].status = 'R';
        }else {
            (*processes)[i].status = 'W';
        }
    }
} 

void calculate_own_dependencies(Process *processes, int process_count) {
    for (int i = 0; i < process_count; i++) {
        int own_dependencies = 0;
        for (int j = 0; j < process_count; j++) {
            for (int k = 0; k < processes[j].n_dependencies; k++) {
                if (processes[j].dependencies[k] == processes[i].id) {
                    own_dependencies++;
                }
            }
        }
        processes[i].own_dependencies = own_dependencies;
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
        if (update_status && processes[i].status != 'E') {
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
                np_time = time;
                np_index = i;
                np_own_dependencies = processes[i].own_dependencies;
            }
            else if (time == np_time && processes[i].own_dependencies > np_own_dependencies) {
                np_time = time;
                np_index = i;
                np_own_dependencies = processes[i].own_dependencies;
            }
        }
    }
    return np_index;
}

void run_on_cores(Process *processes, int process_count, int num_cores) {
    int process_count_finished = 0;
    float time = 0.0f;
    float turnaround[process_count];
    siginfo_t siginfo;
    // pipe
    int fd[process_count][2];
    char read_msg[100];
    double time_used;
    for (int i = 0; i < process_count; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe creation failed");
        }
    }
    while (process_count_finished < process_count) {
        int next_process_index = find_next_process(processes, process_count);
        Process *next_process;
        if (next_process_index == -1 || num_cores == 0) {
            waitid(P_ALL, 0, &siginfo, WEXITED);
            next_process = get_process_by_pid(processes, process_count, siginfo.si_pid);
            if(next_process == NULL) {
                printf("Erro ao buscar pid");
                break;
            }
            read(fd[next_process->id-1][0], read_msg, sizeof(read_msg));
            turnaround[next_process->id] += atof(read_msg);
            time = turnaround[next_process->id];
            printf("Processo %d finalizado | ", next_process->id);
            printf("Turnaround: %f\n", turnaround[next_process->id]);
            finish_process(processes, process_count, next_process->id);
            process_count_finished++;
            num_cores++;
            continue;
        }
        next_process = &processes[next_process_index];
        pid_t pid = fork();
        if (pid == 0) {
            struct timespec start, end;
            close(fd[next_process->id-1][0]);
            clock_gettime(CLOCK_MONOTONIC, &start);
            //char *argv[2];
            //argv[0] = next_process->command;
            //argv[1]=NULL;
            //execv(next_process->command, argv);
            char comando[100];
            sprintf(comando, "./%s", next_process->command);
            system(comando);
            clock_gettime(CLOCK_MONOTONIC, &end);
            time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;;
            snprintf(read_msg, sizeof(read_msg), "%f", time_used);
            write(fd[next_process->id-1][1], read_msg, strlen(read_msg)+1);
            close(fd[next_process->id-1][1]);
            exit(0);
        } else {
            next_process->pid = pid;
            num_cores--;
            turnaround[next_process->id] = time; 
            printf("Executando processo %d\n", next_process->id);
            next_process->status = 'E';
        }
    }
    for (int i = 0; i < process_count; i++) { close(fd[i][1]); }
    for (int i = 0; i < process_count; i++) { wait(NULL); }
    printf("Makespan: %f\n", time);
}

Process * get_process_by_pid(Process * processes, int process_count, pid_t pid) {
    for(int i = 0; i < process_count; i++) {
        if (processes[i].pid == pid) {
            return &processes[i];
        }
    }
    return NULL;
}

void print_processes(Process *processes, int process_count) {
    for (int i = 0; i < process_count; i++) {
        printf("%d, %s, %d, ", processes[i].id, processes[i].command, processes[i].own_dependencies);
        for (int j = 0; j < processes[i].n_dependencies; j++) {
            printf("%d ", processes[i].dependencies[j]);
        }
        printf("\n");
    }
}