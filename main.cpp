#include "matplotlib-cpp/matplotlibcpp.h"
#include "headers/modelling.h"
#include <map>
#include <algorithm>
#include <getopt.h>
#include <cstdlib>
#include <memory>
#include <string>
#include <stdexcept>

namespace plt = matplotlibcpp;

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1;
    if( size <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    std::unique_ptr<char[]> buf( new char[ size ] ); 
    snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 );
}

template <typename T, typename V>
std::vector<T> retrieve_keys(std::map<T, V> map) {
    std::vector<T> result(0);
    for (auto it = map.begin(); it != map.end(); it++)
        result.push_back(it->first);
    return result;    
}

template <typename T, typename V>
std::vector<V> retrieve_values(std::map<T, V> map) {
    std::vector<V> result(0);
    for (auto it = map.begin(); it != map.end(); it++)
        result.push_back(it->second);
    return result;    
}

void draw_plot(std::vector<double> x, std::vector<double> y, std::string inscription) {
    plt::figure_size(1200, 700);
    plt::clf();
    plt::plot(x, y);
    plt::title(inscription);
    while (true)
        plt::pause(0.01);
}

void plot_test1(const Analyzer& results, std::string inscription) {
    std::vector<int> waiting_time_by_task = results.waiting_time_by_task;
    int max_waiting_time_rr = *std::max_element(waiting_time_by_task.begin(), waiting_time_by_task.end());
    std::map<double, double> plot_data_rr = std::map<double, double>();
    for (int i = 0; i <= max_waiting_time_rr; i++)
        plot_data_rr[i] = 0;
    for (auto el : results.waiting_time_by_task)
        plot_data_rr[el]++;
    
    std::vector<double> x_rr = retrieve_keys<double, double>(plot_data_rr);
    std::vector<double> y_rr = retrieve_values<double, double>(plot_data_rr);
    draw_plot(x_rr, y_rr, inscription);
}

void plot_test2(std::map<double, Analyzer> results, std::string inscription) {
    std::vector<double> intensity = retrieve_keys<double, Analyzer>(results);
    std::vector<Analyzer> analyzers = retrieve_values<double, Analyzer>(results);
    std::vector<double> avg_waiting_time(analyzers.size());
    std::transform(analyzers.begin(),
                analyzers.end(),
                avg_waiting_time.begin(),
                [](Analyzer el){return el.get_average_waiting_time();}
    );
    draw_plot(intensity, avg_waiting_time, inscription);
}

void plot_test3(std::map<double, Analyzer> results, std::string inscription) {
    std::vector<double> intensity = retrieve_keys<double, Analyzer>(results);
    std::vector<Analyzer> analyzers = retrieve_values<double, Analyzer>(results);
    std::vector<double> inactivity_percent(analyzers.size());
    std::transform(analyzers.begin(),
                analyzers.end(),
                inactivity_percent.begin(),
                [](Analyzer el){return el.get_inactivity_percent();}
    );
    draw_plot(intensity, inactivity_percent, inscription);
}

struct args {
    char test_number;
    char model_type;
    double intensity;
    int processors = 0;
};

int main(int argc, char* argv[]) {
    args arg;
    int opt;
    
    while ( (opt = getopt(argc,argv,"t:m:i::p::")) !=-1){
		switch (opt) {
		case 't': arg.test_number = *optarg; break;
                case 'm': arg.model_type = *optarg; break;
                case 'i': arg.intensity = atof(optarg); break;
                case 'p': arg.processors = atoi(optarg); break;
        };
    }
    
    double intensity = arg.intensity != 0 ? arg.intensity : 1;
    int processors = arg.processors ? arg.processors : 1;    
    
    if (arg.test_number == '1') {
        Analyzer model;
        std::string inscription = "Залежність кількості заявок від часу очікування при фіксованій інтенсивності вхідного потоку заявок = 1.\n" +
                string_format("кількість процесорів = %d ", processors);
        if (arg.model_type == '1') {
            model = model_round_robin(processors, intensity);
            inscription += "Round Robin";
        }
        else {
            model = model_foreground_background(processors, 10, intensity);
            inscription += "Foreground Background";
        }
        plot_test1(model, inscription);
    }
    else if (arg.test_number >= '2') {
        std::string param_descr_string = string_format("Кількість процесорів = %d, ", processors);
        std::map<double, Analyzer> modelling = std::map<double, Analyzer>();
        std::string inscription;
        if (arg.test_number == '2')
            inscription = "Залежність середнього часу очікування від інтенсивності вхідного потоку заявок.\n" + param_descr_string;
        else if (arg.test_number == '3')
            inscription = "Залежність проценту простою ресурсу від інтенсивності вхідного потоку заявок.\n" + param_descr_string;
                    
        if (arg.model_type == '1')
            inscription += "Round Robin";
        else
            inscription += "Foreground Background";
    
        for (double intensity_test = 0.001; intensity_test < 1; intensity_test += 0.01) {
            if (arg.model_type == '1')
                modelling[intensity_test] = model_round_robin(processors, intensity_test);
            else
                modelling[intensity_test] = model_foreground_background(processors, 10, intensity_test);
        }
        
        std::vector<double> avg_waiting_time;
        for (std::map<double, Analyzer>::iterator it = modelling.begin(); it != modelling.end(); it++)
            avg_waiting_time.push_back(it->second.get_average_waiting_time());
        
        if (arg.test_number == '2')
            plot_test2(modelling, inscription);
        else if (arg.test_number == '3') {
            plot_test3(modelling, inscription);
        }
    }
    return 0;
}
