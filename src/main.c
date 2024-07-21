#include "process.h"

#define INPUT_FILENAME "programs.txt"

int main(int argc, char *argv[]) {
    int process_count;

    Process *processes;

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

    printf("\n\n");

    fclose(entrada);

    calculate_own_dependencies(processes, process_count);

    print_processes(processes, process_count);

    run_on_cores(processes, process_count, num_cores);

    return 0;
}

