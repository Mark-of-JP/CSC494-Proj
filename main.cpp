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

    Nelder_Mead_Solver nms;
    double *nms_solution = nms.solve(&ure, (unsigned) time(NULL));

    PSO_Solver pso(20, 2000, 0.5, 2, 2);
    double *pso_solution = pso.solve(&ure, (unsigned) time(NULL));

    CMA_ES_Solver cma_es(10, 0.5, 0.5, 100);
    double *cma_es_solution = cma_es.solve(&ure, 5406);
    
    std::cout << "Solutions Found!!" << std::endl;
    std::cout << "Nelder Mead Solution (" << nms_solution[0] << ", " << nms_solution[1] << ") -> " << ure.f(nms_solution) << std::endl;
    std::cout << "PSO Solution (" << pso_solution[0] << ", " << pso_solution[1] << ") -> " << ure.f(pso_solution) << std::endl;
    std::cout << "CMA ES Solution (" << cma_es_solution[0] << ", " << cma_es_solution[1] << ") -> " << ure.f(cma_es_solution) << std::endl;

    // StandardPolynomial testing(2, 3, 5406);
    // CMA_ES_Solver cma_es(10, 0.5, 0.5, 100);
    // cma_es.solve(&ure, 5406);

    return 0;
}