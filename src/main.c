#include "process.h"

#define INPUT_FILENAME "programs.txt"

int main(int argc, char *argv[]) {
    // int status;
    int process_count;
    // int tempo = 0;
    // int count = 0;

    Process *processes;

    // siginfo_t siginfo;

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

    print_processes(processes, process_count);

    fclose(entrada);

    calculate_own_dependencies(processes, process_count);

    run_on_single_core(processes, process_count);
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

