#ifndef INPUT_TASK_H
#define INPUT_TASK_H

struct Task {
    int processing_time;
    int left_to_deadline;
    int priority;
    operator bool();
    Task& operator=(const Task& task);
};

Task generate_random_task(int avg_processing_time, int avg_deadline, int avg_priority);

#endif
