#pragma once

#include <iostream>
#include <string>
#include <random>
#include <cstdlib>

#include "../problems/opt_problem.h"

typedef unsigned int uint;

class OptSolver {
    public:
        virtual double* solve(OptProblem *optProblem, uint randomSeed) = 0;
};