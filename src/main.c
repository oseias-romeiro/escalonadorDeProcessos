#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILENAME "programs.txt"

typedef struct {
    int id;
    char command;
    int dependencies[];
} Process;


int main(int argc, char *argv[]) {
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

    char buff[256];
    int c = 0;
    while (fgets(buff, sizeof(buff), entrada) != NULL) {
        printf("%s", buff);
        c++;
    }

    return 0;
}

