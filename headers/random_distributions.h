#ifndef RANDOM_DISTRIBUTIONS_H
#define RANDOM_DISTRIBUTIONS_H

#include <math.h>
#include <time.h>

int uniform_distr_int(int min_val, int max_val);

double uniform_distr_double(int min_val, int max_val);

int exponential_distr(double lambda);

int poisson_distr(double lambda);

#endif /* RANDOM_DISTRIBUTIONS_H */
