#include "processor.h"
#include <algorithm>

double Analyzer::get_average_queue_size() {
    return (double) total_queue_size_tick / ticks_count;
}

double Analyzer::get_average_waiting_time() {
    double sum = 0;
    for (int x : waiting_time_by_task)
        sum += (double) x;
    /*std::for_each(waiting_time_by_task.begin(), waiting_time_by_task.end(), [&] (int x) {
        sum += (double) x / waiting_time_by_task.size();
    });*/
    return sum / waiting_time_by_task.size();
}

double Analyzer::get_inactivity_percent() {
    return (double) inactivity_ticks_count / ticks_count;
}

void Processor::next_tick() {
    if (curr_task) {
        curr_task.left_to_deadline--;
        curr_task.processing_time--;
        time_quantum_left--;
        if (curr_task.processing_time == 0) {
            analyzer.waiting_time_by_task.push_back(curr_task.waiting_time);
            curr_task = {0, 0, 0};
            active = false;
            analyzer.completed_tasks_count++;
        }
        else if (time_quantum_left == 0) {
            queue->push_back(curr_task);
            curr_task = {0, 0};
            active = false;
        }
        else if (curr_task.left_to_deadline == 0) {
            curr_task = {0, 0};
            active = false;
            analyzer.failed_tasks_count++;
        }
    }
    else {
        if (!queue->empty()) {
            curr_task = queue->front();
            queue->erase(queue->begin());
            active = true;
            time_quantum_left = time_quantum;
        }
    }
    if (!queue->empty()) {
        for (Task &task : *queue) {
            task.waiting_time++;
            task.left_to_deadline--;
            if (task.left_to_deadline <= 0)
                analyzer.failed_tasks_count++;
        }
        auto new_end = std::remove_if(queue->begin(), queue->end(),
                [](Task el){return el.left_to_deadline <= 0;});
        queue->erase(new_end, queue->end());
    }
}

/*int Processor::get_failed_tasks_count() {
    return failed_tasks_count;
}*/

void Processor::set_task(Task& task) {
    curr_task = task;
    time_quantum_left = time_quantum;
}

Processor::Processor(std::vector<Task>& queue, int time_quantum, Analyzer& analyzer) {
    this->queue = &queue;
    this->time_quantum = time_quantum;
    this->analyzer = analyzer;
    curr_task = {0, 0, 0};
    //this->analyzer.waiting_time_by_task = std::vector<int>(0);
}

Processor::Processor() {
    this->queue = nullptr;
    this->time_quantum = 0;
    //this->analyzer = NULL;
    curr_task = {0, 0, 0};
}

/*int Processor::get_completed_tasks_count() {
    return analyzer->completed_tasks_count;
}*/

ForegroundBackgroundProcessor::ForegroundBackgroundProcessor(std::vector<Task>* queue, int priorities_count, int time_quantum, Analyzer& analyzer) : Processor() {
    this->queue = queue;
    this->time_quantum = time_quantum;
    this->priorities_total = priorities_count;
    this->analyzer = analyzer;
    //failed_tasks_count = 0;
    curr_task = {0, 0, 0};
    this->analyzer.waiting_time_by_task = std::vector<int>(0);
}

void ForegroundBackgroundProcessor::next_tick() {
    if (curr_task) {
        curr_task.left_to_deadline--;
        curr_task.processing_time--;
        time_quantum_left--;
        if (curr_task.processing_time == 0) {
            analyzer.waiting_time_by_task.push_back(curr_task.waiting_time);
            curr_task = {0, 0};
            active = false;
            analyzer.completed_tasks_count++;
            
        }
        else if (time_quantum_left == 0) {
            if (curr_task.priority + 1 < priorities_total)
                curr_task.priority++;
            queue[curr_task.priority - 1].push_back(curr_task);
            curr_task = {0, 0};
            active = false;
        }
        else if (curr_task.left_to_deadline == 0) {
            curr_task = {0, 0};
            active = false;
            analyzer.failed_tasks_count++;
        }
    }
    else {
        int max_priority = get_max_priority_queue_available();
        if (max_priority > -1) {
            curr_task = queue[max_priority].front();
            queue[max_priority].erase(queue[max_priority].begin());
            active = true;
            time_quantum_left = time_quantum;
        }
    }
    for (int i = 0; i < priorities_total; i++)
        if (!queue[i].empty()) {
            for (Task &task : queue[i]) {
                task.left_to_deadline--;
                task.waiting_time++;
                if (task.left_to_deadline <= 0)
                    analyzer.failed_tasks_count++;
            }
            auto new_end = std::remove_if(queue[i].begin(), queue[i].end(),
                    [](Task el){return el.left_to_deadline <= 0;});
            queue[i].erase(new_end, queue[i].end());
        }
}

int ForegroundBackgroundProcessor::get_max_priority_queue_available() {
    for (int i = 0; i < priorities_total; i++)
        if (!queue[i].empty())
            return i;
    return -1;
}
