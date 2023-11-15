#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <cstdlib>
#include <vector>

typedef unsigned int uint;

enum class OptimizationType {
    CONTINUOUS,
    DISCRETE
};

class OptProblem {
    public:
        OptProblem(OptimizationType optType) : optimization_type(optType) {}

        virtual double f_implement(double *input) = 0;
        virtual int getInputDimension() = 0;
        virtual double* getInputConstraints() = 0;

        virtual double** generateRandomFeasibleInputs(uint num_of_inputs, uint randomSeed) = 0;
        virtual void writeToFile(std::string path) = 0;

        double f(double *input, bool incrementCounter = true) {
            
            if (incrementCounter) {
                this->time_called++;
            }
            
            return f_implement(input);
        };

        void resetCounter() { this->time_called = 0; };
        uint getNumCalled() { return this->time_called; }

    private:
        OptimizationType optimization_type;
        uint time_called = 0;
};