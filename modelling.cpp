#include "modelling.h"
#include <cstdlib>
#include <stdio.h>
#include "random_distributions.h"
#include "processor.h"
#include <algorithm>

Task* spawn_processes(unsigned int count, int max_priority, int avg_processing_time, int avg_deadline) {
    Task* random_processes = new Task[count];
    for (int i = 0; i < count; i++) {
        random_processes[i] = generate_random_task(avg_processing_time, avg_deadline);
    }
    return random_processes;
}

int* spawn_processes_coming_time(unsigned int count, double intensity) {
    int* time_process_distance = new int[count];
    for (int i = 0; i < count; i++) {
        time_process_distance[i] = exponential_distr(intensity) + 1;
    }
    return time_process_distance;
}

void model_round_robin(unsigned int proc_count) {
    srand(time(NULL));
    int processes_count = 100;
    double intensity = 0.5;
    int* time_process_distance = spawn_processes_coming_time(processes_count, intensity);
    Task* random_processes = spawn_processes(processes_count, 1, 5, 20);
    
    std::vector<Task> queue(0);
    int new_process_tick = 0;
    int j = 0;
    int total_ticks = 0;
    //bool prev_tick_active = false;
    Processor proc = Processor(queue, 3);
    Analyzer analyzer = {0, 0, processes_count};
    
    std::vector<Processor> processors(proc_count, proc);
    std::vector<Analyzer> analyzers(proc_count, analyzer);
    std::vector<bool> prev_tick_active(proc_count, false);
    
    while (j < processes_count || !queue.empty()) {
        if (new_process_tick == 0) {
            queue.push_back(random_processes[j]);
            new_process_tick = time_process_distance[j];
            j++;
        }
        total_ticks++;
        
        for (int i = 0; i < proc_count; i++) {
            processors[i].next_tick();
            bool curr_tick_active = processors[i].active;
            if ((curr_tick_active || prev_tick_active[i]) == false)
                analyzers[i].inactivity_ticks_count++;
            prev_tick_active[i] = curr_tick_active;
        }
        new_process_tick--;
    }
    for (int i = 0; i < proc_count; i++) {
        printf("Failed tasks count: %d\n", processors[i].get_failed_tasks_count());
        printf("Inactivity ticks count: %d\n\n", analyzers[i].inactivity_ticks_count);
    }
    printf("\nTotal ticks count: %d", total_ticks);
}

void model_foreground_background(unsigned int proc_count) {
    
}
