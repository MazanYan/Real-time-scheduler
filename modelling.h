#include "processor.h"

#ifndef MODELLING_H
#define MODELLING_H

Task* spawn_processes(unsigned int count, int avg_processing_time, int avg_deadline, int max_priority);
int* spawn_processes_coming_time(unsigned int count, double intensity);
void model_round_robin(int proc_count, double intensity);
bool priority_queue_empty(std::vector<Task>* queue, int queue_length);
void model_foreground_background(int proc_count, int max_priority, double intensity);


#endif /* MODELLING_H */

