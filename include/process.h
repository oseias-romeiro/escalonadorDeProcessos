#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "process.h"

typedef struct {
    int id;
    char command[8];
    int * dependencies;
    int n_dependencies;
} Process;

void map_process(Process** processes, int * process_count, FILE *file);

#endif