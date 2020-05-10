#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include "input_task.h"
#include <thread>
#include <mutex>

extern std::mutex queue_mutex;

class Processor {
protected:
    std::vector<Task>* queue;
    int time_quantum;
    int time_quantum_left;
    Task curr_task;
    int failed_tasks_count;
    int completed_tasks_count;
public:
    Processor(std::vector<Task>& queue, int time_quantum);
    Processor();
    virtual void next_tick();
    void set_task(Task& task);
    bool active;
    int get_failed_tasks_count();
    int get_completed_tasks_count();
};

class ForegroundBackgroundProcessor : public Processor {
protected:
    std::vector<Task>* queue;
private:
    int get_max_priority_queue_available();
    int priorities_total;
public:
    ForegroundBackgroundProcessor(std::vector<Task>* queue, int priorities_count, int time_quantum);
    void next_tick() override;
};

struct Analyzer {
    int inactivity_ticks_count;
    int failed_tasks_count;
    int total_tasks_count;
};

#endif
