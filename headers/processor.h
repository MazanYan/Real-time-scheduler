#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include "input_task.h"

struct Analyzer {
    int inactivity_ticks_count;
    int failed_tasks_count;
    int completed_tasks_count;
    int total_tasks_count;
    double total_queue_size_tick;
    int ticks_count;
    std::vector<int> waiting_time_by_task;
    double get_average_queue_size();
    double get_average_waiting_time();
    double get_inactivity_percent();
};


class Processor {
protected:
    std::vector<Task>* queue;
    int time_quantum;
    int time_quantum_left;
    Task curr_task;
public:
    Processor(std::vector<Task>& queue, int time_quantum, Analyzer& analyzer);
    Processor();
    virtual void next_tick();
    void set_task(Task& task);
    bool active;
    Analyzer analyzer;
};

class ForegroundBackgroundProcessor : public Processor {
protected:
    std::vector<Task>* queue;
private:
    int get_max_priority_queue_available();
    int priorities_total;
public:
    ForegroundBackgroundProcessor(std::vector<Task>* queue, int priorities_count, int time_quantum, Analyzer& analyzer);
    void next_tick() override;
};

#endif /* PROCESSOR_H */
