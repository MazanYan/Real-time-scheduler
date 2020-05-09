#include "random_distributions.h"

int uniform_distr_int(int min_val, int max_val) {
    return rand() % max_val + min_val;
}

double uniform_distr_double(int min_val, int max_val) {
    double random01 = ((double) rand() / __INT32_MAX__);
    return random01 * max_val + min_val;
}

int exponential_distr(double lambda) {
    double u = uniform_distr_double(0, 1);
    double l = log(1 - u) / (-lambda);
    return l;
}

int poisson_distr(double lambda) {
    double exp_rate_inv = exp(-lambda);
    double k = 0, prod = uniform_distr_double(0, 1);
    while (prod > exp_rate_inv) {
        prod *= uniform_distr_double(0, 1);
        ++k;
    }
    return k;
}