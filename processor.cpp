#include "processor.h"
#include <algorithm>

void Processor::next_tick() {
    if (curr_task) {
        curr_task.left_to_deadline--;
        curr_task.processing_time--;
        time_quantum_left--;
        if (curr_task.processing_time == 0) {
            curr_task = {0, 0, 0};
            active = false;
            completed_tasks_count++;
        }
        else if (time_quantum_left == 0) {
            queue->push_back(curr_task);
            curr_task = {0, 0};
            active = false;
        }
        else if (curr_task.left_to_deadline == 0) {
            curr_task = {0, 0};
            active = false;
            failed_tasks_count++;
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
        for (auto &task : *queue) {
            task.left_to_deadline--;
            if (task.left_to_deadline <= 0)
                failed_tasks_count++;
        }
        auto new_end = std::remove_if(queue->begin(), queue->end(),
                [](Task el){return el.left_to_deadline <= 0;});
        queue->erase(new_end, queue->end());
    }
}

int Processor::get_failed_tasks_count() {
    return failed_tasks_count;
}

void Processor::set_task(Task& task) {
    curr_task = task;
    time_quantum_left = time_quantum;
}

Processor::Processor(std::vector<Task>& queue, int time_quantum) {
    this->queue = &queue;
    this->time_quantum = time_quantum;
    failed_tasks_count = 0;
    curr_task = {0, 0, 0};
    completed_tasks_count = 0;
}

Processor::Processor() {
    this->queue = nullptr;
    this->time_quantum = 0;
    failed_tasks_count = 0;
    curr_task = {0, 0, 0};
    completed_tasks_count = 0;
}

int Processor::get_completed_tasks_count() {
    return completed_tasks_count;
}

ForegroundBackgroundProcessor::ForegroundBackgroundProcessor(std::vector<Task>* queue, int priorities_count, int time_quantum) : Processor() {
    this->queue = queue;
    this->time_quantum = time_quantum;
    this->priorities_total = priorities_count;
    failed_tasks_count = 0;
    curr_task = {0, 0, 0};
    completed_tasks_count = 0;
}

void ForegroundBackgroundProcessor::next_tick() {
    if (curr_task) {
        curr_task.left_to_deadline--;
        curr_task.processing_time--;
        time_quantum_left--;
        if (curr_task.processing_time == 0) {
            curr_task = {0, 0};
            active = false;
            completed_tasks_count++;
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
            failed_tasks_count++;
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
            for (auto &task : queue[i]) {
                task.left_to_deadline--;
                if (task.left_to_deadline <= 0)
                    failed_tasks_count++;
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
