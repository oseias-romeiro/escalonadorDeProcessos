/*
    - Autores:
        - David Herbert de Souza Brito - 200057405
        - Gustavo Lopes Dezan - 202033463
        - Julia Yuri Garcia Baba - 190057921
        - Oseias Romeiro Magalhães - 211036123
    
    - Implementação do Escalonador de Workflow de Processos, o qual é do tipo Shorted Job First
    
    - Versão do compilador: gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.1) 

    - Sistema Operacional: Ubuntu 20.04.3 LTS
*/

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

