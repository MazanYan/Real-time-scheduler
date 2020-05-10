#include <cstdlib>
#include <stdio.h>
#include "random_distributions.h"
#include "processor.h"

Task* spawn_processes(unsigned int count, int avg_processing_time, int avg_deadline, int max_priority = 1) {
    Task* random_processes = new Task[count];
    for (int i = 0; i < count; i++) {
        Task random_proc = generate_random_task(avg_processing_time, avg_deadline, max_priority);
        random_processes[i] = random_proc;
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

void model_round_robin(int proc_count, double intensity) {
    srand(time(NULL));
    int processes_count = 1000;
    int* time_process_distance = spawn_processes_coming_time(processes_count, intensity);
    Task* random_processes = spawn_processes(processes_count, 5, 5);
    
    std::vector<Task> queue(0);
    int new_process_tick = 0;
    int j = 0;
    int total_ticks = 0;
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
        printf("Failed tasks count: %d (total completed - %d)\n", processors[i].get_failed_tasks_count(), processors[i].get_completed_tasks_count());
        printf("Inactivity ticks count: %d\n\n", analyzers[i].inactivity_ticks_count);
    }
    printf("\nTotal ticks count: %d", total_ticks);
}

bool priority_queue_empty(std::vector<Task>* queue, int queue_length) {
    for (int i = 0; i < queue_length; i++) {
        if (!queue[i].empty())
            return false;
    }
    return true;
}

void model_foreground_background(int proc_count, int max_priority, double intensity) {
    srand(time(NULL));
    int processes_count = 1000;
    int* time_process_distance = spawn_processes_coming_time(processes_count, intensity);
    Task* random_processes = spawn_processes(processes_count, 5, 5, max_priority);
    
    std::vector<Task>* queue = new std::vector<Task>[max_priority];
    for (int i = 0; i < proc_count; i++)
        queue[i] = std::vector<Task>(0);
    
    int new_process_tick = 0;
    int j = 0;
    int total_ticks = 0;
    ForegroundBackgroundProcessor proc = ForegroundBackgroundProcessor(queue, max_priority, 3);
    Analyzer analyzer = {0, 0, processes_count};
    
    std::vector<ForegroundBackgroundProcessor> processors(proc_count, proc);
    std::vector<Analyzer> analyzers(proc_count, analyzer);
    std::vector<bool> prev_tick_active(proc_count, false);
    
    while (j < processes_count || !priority_queue_empty(queue, max_priority)) {
        if (new_process_tick == 0) {
            Task process = random_processes[j];
            queue[process.priority - 1].push_back(process);
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
        printf("Failed tasks count: %d (total completed - %d)\n", processors[i].get_failed_tasks_count(), processors[i].get_completed_tasks_count());
        printf("Inactivity ticks count: %d\n\n", analyzers[i].inactivity_ticks_count);
    }
    printf("\nTotal ticks count: %d", total_ticks);
}
