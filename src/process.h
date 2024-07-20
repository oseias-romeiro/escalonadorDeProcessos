#include <stdio.h>
#
#ifndef PROCESS_H
#define PROCESS_H
#define MAX_INT 1000000


typedef struct process Process;

void map_process(Process** processes, int * process_count, FILE *file);

void calculate_own_dependencies(Process *processes, int process_count);

void update_dependencies(Process *processes, int process_count, int id);

void finish_process(Process *processes, int process_count, int id);

int find_next_process(Process *processes, int process_count);

void run_on_single_core(Process *processes, int process_count);

void print_processes(Process *processes, int process_count);


#endif