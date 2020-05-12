# Real-time-scheduler
A computational work for university.
This is a multi-processor simulator of real-time scheduling using Round-Robin and Foreground-Background strategies.

The project uses matplotlib C++ binding that can be found by the link https://github.com/lava/matplotlib-cpp
To run the program ensure that you have all dependencies of matplotlic-cpp installed.

To run the project clone it, go to cloned directory, run ```make``` and then run ```./main``` with command line arguments

# Command line arguments
1. ```-t``` - an obligatory parameter. Type of plot to build. Description of possible values of parameter ```-t```:
    * ```1``` - Dependence of the number of applications on the waiting time at a fixed intensity of the incoming flow of queries = 1
    (Orig: Залежність кількості заявок від часу очікування при фіксованій інтенсивності вхідного потоку заявок = 1)
    * ```2``` - Dependence of the average waiting time on the intensity of the incoming flow of queries
    (Orig: Залежність середнього часу очікування від інтенсивності вхідного потоку заявок)
    * ```3``` - Dependence of the percentage of downtime of the resource on the intensity of the incoming flow of applications
    (Orig: Залежність проценту простою ресурсу від інтенсивності вхідного потоку заявок)
2. ```-m``` - an obligatory parameter. Type of strategy to model
    * ```1``` - Round-Robin
    * ```2``` - Foreground-Background
3. ```-i``` - intensity of queries stream (parameter is a double number, set default to 1.0)
4. ```-p``` - quantity of processors in real-time scheduler, set default to 1
