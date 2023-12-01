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

    Nelder_Mead_Solver nms(3);
    double *nms_solution = nms.solve(&ure, (unsigned) time(NULL));

    PSO_Solver pso(20, 2000, 0.5, 2, 2);
    double *pso_solution = pso.solve(&ure, (unsigned) time(NULL));

    CMA_ES_Solver cma_es(10, 0.5, 0.5, 1000);
    double *cma_es_solution = cma_es.solve(&ure, 5406);
    
    std::cout << "Solutions Found!!" << std::endl;
    std::cout << "Nelder Mead Solution (" << nms_solution[0] << ", " << nms_solution[1] << ") -> " << ure.f(nms_solution) << std::endl;
    std::cout << "PSO Solution (" << pso_solution[0] << ", " << pso_solution[1] << ") -> " << ure.f(pso_solution) << std::endl;
    std::cout << "CMA ES Solution (" << cma_es_solution[0] << ", " << cma_es_solution[1] << ") -> " << ure.f(cma_es_solution) << std::endl;

    // StandardPolynomial testing(2, 3, 5406);
    // CMA_ES_Solver cma_es(10, 0.5, 0.5, 100);
    // cma_es.solve(&ure, 5406);

    int num_of_terms = 2;
    int num_of_zeroes = 2;

    StandardPolynomial sp = StandardPolynomial(num_of_terms, num_of_zeroes, (unsigned) time(NULL));
    std::vector<int> term_index;
    std::vector<double> lower_bound;
    std::vector<double> upper_bound;
    std::cout << "STANDARD POLY CREATED" << std::endl; 
    sp.SetRandomConstraints(term_index, lower_bound, upper_bound, (unsigned) time(NULL));

    double *best_input = sp.getBestInput();
    double *worst_input = sp.getBestInput(false);

    sp.PrintEquation();

    std::cout << "\nCoords for best input are: (";
    for(int i = 0; i < num_of_terms; i++) {
        std::cout << best_input[i] << ", ";
    }
    std::cout << ")" << std::endl;

    std::cout << "The Best Input is " << sp.f(best_input) << std::endl;
    std::cout << "The Worst Input is " << sp.f(worst_input) << std::endl;


    std::cout << "Solutions Found!!" << std::endl;

    Nelder_Mead_Solver nms2(3);
    nms_solution = nms2.solve(&sp, (unsigned) time(NULL));
    std::cout << "Nelder Mead Solution (" << nms_solution[0] << ", " << nms_solution[1] << ", " << nms_solution[2] << ") -> " << sp.f(nms_solution) << std::endl;

    PSO_Solver pso2(20, 2000, 0.5, 2, 2);
    std::cout << "PSO Solution (" << pso_solution[0] << ", " << pso_solution[1] << ", " << pso_solution[2] << ") -> " << sp.f(pso_solution) << std::endl;
    pso_solution = pso2.solve(&sp, (unsigned) time(NULL));

    CMA_ES_Solver cma_es2(10, 1, 0.5, 100);
    cma_es_solution = cma_es2.solve(&sp, 5406);
    std::cout << "CMA ES Solution (" << cma_es_solution[0] << ", " << cma_es_solution[1] << ", " << cma_es_solution[2] << ") -> " << sp.f(cma_es_solution) << std::endl;
    
    
    
    


    return 0;
}