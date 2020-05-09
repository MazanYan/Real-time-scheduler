#include "processor.h"
#include <algorithm>

void Processor::next_tick() {
    if (curr_task) {
        curr_task.left_to_deadline--;
        curr_task.processing_time--;
        time_quantum_left--;
        if (time_quantum_left == 0) {
            queue->push_back(curr_task);
            curr_task = {0, 0};
            active = false;
        }
        else if (curr_task.processing_time == 0) {
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
            //state = state_used;
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
    curr_task = {0, 0};
}
