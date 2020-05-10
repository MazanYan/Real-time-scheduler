//#include "matplotlib-cpp/matplotlibcpp.h"
#include "modelling.h"

using namespace std;

int main(int argc, char** argv) {
    printf("\n\nModelling RR:\n");
    model_round_robin(10, 1);
    printf("\n\nModelling FB:\n");
    model_foreground_background(10, 10, 1);
    return 0;
}
