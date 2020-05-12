#include "processor.h"
#include <vector>
#include <map>


#ifndef MODELLING_H
#define MODELLING_H

std::vector<Task> spawn_processes(unsigned int count, int avg_processing_time, int avg_deadline, int max_priority);
std::vector<int> spawn_processes_coming_time(unsigned int count, double intensity);
Analyzer model_round_robin(int proc_count, double intensity);
bool priority_queue_empty(std::vector<Task>* queue, int queue_length);
Analyzer model_foreground_background(int proc_count, int max_priority, double intensity);

#endif /* MODELLING_H */

