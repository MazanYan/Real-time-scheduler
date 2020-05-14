#include "headers/random_distributions.h"
#include "headers/processor.h"

std::vector<Task> spawn_processes(unsigned int count, int avg_processing_time, int avg_deadline, int max_priority = 1) {
    std::vector<Task> random_processes;
    for (int i = 0; i < count; i++)
        random_processes.push_back(generate_random_task(avg_processing_time, avg_deadline, max_priority));
    return random_processes;
}

std::vector<int> spawn_processes_coming_time(unsigned int count, double intensity) {
    std::vector<int> time_process_distance;
    for (int i = 0; i < count; i++)
        time_process_distance.push_back(exponential_distr(intensity) + 1);
    return time_process_distance;
}

Analyzer model_round_robin(int proc_count, double intensity) {
    srand(time(NULL));
    int processes_count = 1000;
    std::vector<int> time_process_distance = spawn_processes_coming_time(processes_count, intensity);
    std::vector<Task> random_processes = spawn_processes(processes_count, 2, 5);
    
    std::vector<Task> queue(0);
    int new_process_tick = 0;
    int j = 0;
    Analyzer analyzer = {0, 0, 0, 0, 0, 0, std::vector<int>(1, 0)};
    Processor proc = Processor(queue, 3, analyzer);
    std::vector<Processor> processors(0);
    for (int i = 0; i < proc_count; i++) {
        Analyzer analyzer = {0, 0, 0, 0, 0, 0, std::vector<int>(3, 0)};
        processors.push_back(Processor(queue, 3, analyzer));
    }
    std::vector<bool> prev_tick_active(proc_count, false);
    
    while (j < processes_count || !queue.empty()) {
        if (new_process_tick == 0 && j < processes_count) {
            queue.push_back(random_processes[j]);
            new_process_tick = time_process_distance[j];
            j++;
        }
        analyzer.ticks_count++;
        analyzer.total_queue_size_tick += queue.size();
        
        for (int i = 0; i < proc_count; i++) {
            processors[i].next_tick();
            bool curr_tick_active = processors[i].active;
            if ((curr_tick_active || prev_tick_active[i]) == false)
                processors[i].analyzer.inactivity_ticks_count++;
            prev_tick_active[i] = curr_tick_active;
        }
        new_process_tick--;
    }
    for (int i = 0; i < proc_count; i++) {
        analyzer.completed_tasks_count += processors[i].analyzer.completed_tasks_count;
        analyzer.failed_tasks_count += processors[i].analyzer.failed_tasks_count;
        analyzer.inactivity_ticks_count += processors[i].analyzer.inactivity_ticks_count;
        analyzer.waiting_time_by_task.insert(std::end(analyzer.waiting_time_by_task),
                std::begin(processors[i].analyzer.waiting_time_by_task),
                std::end(processors[i].analyzer.waiting_time_by_task));
    }
    return analyzer;
}

bool priority_queue_empty(std::vector<Task>* queue, int queue_length) {
    for (int i = 0; i < queue_length; i++) {
        if (!queue[i].empty())
            return false;
    }
    return true;
}

Analyzer model_foreground_background(int proc_count, int max_priority, double intensity) {
    srand(time(NULL));
    int processes_count = 1000;
    std::vector<int> time_process_distance = spawn_processes_coming_time(processes_count, intensity);
    std::vector<Task> random_processes = spawn_processes(processes_count, 2, 5);
    
    std::vector<Task>* queue = new std::vector<Task>[max_priority];
    for (int i = 0; i < proc_count; i++)
        queue[i] = std::vector<Task>(0);
    
    int new_process_tick = 0;
    int j = 0;
    Analyzer analyzer = {0, 0, 0, 0, 0, 0, std::vector<int>(0)};
    ForegroundBackgroundProcessor proc = ForegroundBackgroundProcessor(queue, max_priority, 3, analyzer);
    
    std::vector<ForegroundBackgroundProcessor> processors(proc_count, proc);
    std::vector<Analyzer> analyzers(proc_count, analyzer);
    std::vector<bool> prev_tick_active(proc_count, false);
    
    while (j < processes_count || !priority_queue_empty(queue, max_priority)) {
        if (new_process_tick == 0 && j < processes_count) {
            Task process = random_processes[j];
            queue[process.priority - 1].push_back(process);
            new_process_tick = time_process_distance[j];
            j++;
        }
        analyzer.ticks_count++;
        for (int i = 0; i < proc_count; i++) {
            analyzer.total_queue_size_tick += queue[i].size();
        }
        
        for (int i = 0; i < proc_count; i++) {
            processors[i].next_tick();
            processors[i].analyzer.ticks_count++;
            bool curr_tick_active = processors[i].active;
            if ((curr_tick_active || prev_tick_active[i]) == false)
                processors[i].analyzer.inactivity_ticks_count++;
            prev_tick_active[i] = curr_tick_active;
        }
        new_process_tick--;
    }
    analyzer.ticks_count = 0;
    for (int i = 0; i < proc_count; i++) {
        analyzer.completed_tasks_count += processors[i].analyzer.completed_tasks_count;
        analyzer.failed_tasks_count += processors[i].analyzer.failed_tasks_count;
        analyzer.inactivity_ticks_count += processors[i].analyzer.inactivity_ticks_count;
        analyzer.ticks_count += processors[i].analyzer.ticks_count;
        analyzer.waiting_time_by_task.insert(std::end(analyzer.waiting_time_by_task),
                std::begin(processors[i].analyzer.waiting_time_by_task),
                std::end(processors[i].analyzer.waiting_time_by_task));
    }
    return analyzer;
}
