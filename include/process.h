#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#ifndef PROCESS_H
#define PROCESS_H
#define MAX_INT 1000000


typedef struct process Process;

void map_process(Process** processes, int * process_count, FILE *file);

void calculate_own_dependencies(Process *processes, int process_count);

void update_dependencies(Process *processes, int process_count, int id);

void finish_process(Process *processes, int process_count, int id);

int find_next_process(Process *processes, int process_count);

void run_on_cores(Process *processes, int process_count, int num_cores);

void print_processes(Process *processes, int process_count);

Process * get_process_by_pid(Process * processes, int process_count, pid_t pid);


#endif