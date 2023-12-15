#pragma once

#include <iostream>
#include <string>
#include <random>
#include <cstdlib>
#include <chrono>

#include "../problems/opt_problem.h"

typedef unsigned int uint;

class OptSolver {
    public:
        virtual double* solve(OptProblem *optProblem, uint randomSeed, std::vector<double> *best_answer, std::vector<int> *num_of_f_calls, std::vector<int> *num_of_time_passed) = 0;
};