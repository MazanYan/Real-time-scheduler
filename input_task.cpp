#include "input_task.h"
#include "random_distributions.h"

Task::operator bool() {
    if (processing_time == 0 && left_to_deadline == 0) return false;
    else return true;
}

Task& Task::operator=(const Task& task) {
    processing_time = task.processing_time;
    left_to_deadline = task.left_to_deadline;
    priority = task.priority;
    waiting_time = task.waiting_time;
    return *this;
}

Task generate_random_task(int avg_processing_time, int avg_deadline, int max_priority = 1) {
    int processing_time = poisson_distr(avg_processing_time);
    if (!processing_time)
        processing_time++;
    int deadline = poisson_distr(avg_deadline) + processing_time;
    int priority = uniform_distr_int(1, max_priority);
    Task result = {processing_time, deadline, priority, 0};
    return result;
}
