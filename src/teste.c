#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define INPUT_FILENAME "programs.txt"

typedef struct {
    int id;
    char command;
    int dependencies[];
} Process;


int main(int argc, char *argv[]) {
    int status;
    int tempo = 0;
    int count = 0;

    siginfo_t siginfo;

    if (argc != 2) {
        perror("Uso: escalona <numero_de_cores>");
        return 1;
    }
    const int num_cores = atoi(argv[1]);
    printf("Numero de cores: %d\n", num_cores);

    for(int i = 0; i < 3; i++) {
        pid_t pid = fork();
        if(pid==0){
            static char *argv[]={"teste15", NULL};
            execv("teste15.out", argv);
            printf("erro");
            exit(127);
        } 
    }

    // while(1){
    //     waitpid(-1,&status,0); //espera o processo filho terminar
    //     printf("%d\n",WEXITSTATUS(status));
    //     tempo += 15;
    //     count++;

    //     if(count == 3){
    //         break;
    //     }
    // }

    while(1){
        waitid(P_ALL, 0, &siginfo, WEXITED);
        printf("%d\n", siginfo.si_pid);
        count++;
        if(count == 3){
            break;
        }
    }
   
    printf("tempo: %d\n", tempo);
    
    
    // FILE *entrada = fopen(INPUT_FILENAME, "r");
    // if (entrada == NULL) {
    //     fclose(entrada);
    //     perror("Erro ao abrir o arquivo de entrada!");
    //     return 1;
    // }

    // char buff[256];
    // int c = 0;
    // while (fgets(buff, sizeof(buff), entrada) != NULL) {
    //     printf("%s", buff);
    //     c++;
    // }

    return 0;
}

