#include <iostream>
#include <string>
#include <random>
#include <cstdlib>

#include "main.h"

int main() {

    // Problem Set Up

    // CFLP too difficult as of rn
    UncapacitatedRandomEquation ure = UncapacitatedRandomEquation();

    double ure_input[2] = { 0, -2.7928 };
    double ure_result = ure.f(ure_input, false);

    // std::cout << "\nUncapacitatedRandomEquation minimum value is: " << ure_result << std::endl;

    // Nelder_Mead_Solver nms;

    // nms.solve(&ure, (unsigned) time(NULL));

    // std::cout << ure.getNumCalled() << std::endl;

    // // Particle Swarm
    // std::cout << "\nParticle Swarm: " << std::endl;
    // ure.resetCounter();

    // PSO_Solver pso(20, 2000, 0.5, 2, 2);
    // pso.solve(&ure, (unsigned) time(NULL));

    CMA_ES_Solver cma_es(10, 0.5, 0.5, 100);
    cma_es.solve(&ure, 5406);

    return 0;
}