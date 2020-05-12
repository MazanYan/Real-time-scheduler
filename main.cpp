//#include "matplotlib-cpp/matplotlibcpp.h"
#include "modelling.h"
#include <map>
#include <algorithm>

using namespace std;

int main(int argc, char** argv) {
    Analyzer modelling_round_robin = model_round_robin(1, 1);
    Analyzer modelling_foreground_background = model_foreground_background(10, 10, 1);
    
    printf("Avg waiting time RR: %f", modelling_round_robin.get_average_waiting_time());
    // Test 1 Графік залежності кількості заявок від часу очікування при фіксованій інтенсивності вхідного потоку заявок.
    int max_waiting_time = *std::max_element(modelling_round_robin.waiting_time_by_task.begin(), modelling_round_robin.waiting_time_by_task.end());
    std::map<int, int> plot_data = std::map<int, int>();
    for (int i = 0; i <= max_waiting_time; i++)
        plot_data[i] = 0;
    for (auto el : modelling_round_robin.waiting_time_by_task)
        plot_data[el]++;
    return 0;
}
