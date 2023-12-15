#include <iostream>
#include <string>
#include <random>
#include <cstdlib>

#include "main.h"

int main() {

    int num_of_repetitions_per_level = 25;
    int max_num_of_terms = 7;
    int max_num_of_zeroes = 20;

    // Experiment Experiment Set Up
    int num_of_problems = 1;
    int num_of_solvers = 3;

    std::cout << num_of_problems << ", " << num_of_solvers << ", " << num_of_repetitions_per_level << std::endl;

    for (int num_of_terms = 2; num_of_terms < max_num_of_terms; num_of_terms++) {
        for (int num_of_zeroes = 2; num_of_zeroes < max_num_of_zeroes; num_of_zeroes++) {

            for (int rep_index = 0; rep_index < num_of_repetitions_per_level; rep_index++) {

                    // Problem Set Up

                    
                    int num_of_iterations = 200;

                    StandardPolynomial sp = StandardPolynomial(num_of_terms, num_of_zeroes, 7 * rep_index + (unsigned) time(NULL));
                    std::vector<int> term_index;
                    std::vector<double> lower_bound;
                    std::vector<double> upper_bound;

                    sp.SetRandomConstraints(term_index, lower_bound, upper_bound, (unsigned) time(NULL));

                    double *best_input = sp.getBestInput();
                    double *worst_input = sp.getBestInput(false);

                    

                    try {

                    std::vector<double> best_guess;
                    std::vector<int> num_of_f_calls;
                    std::vector<int> time_passed;

                    RandomSelection rs(num_of_terms * 5);
                    double *random_solution = rs.solve(&sp, rep_index * 4, &best_guess, &num_of_f_calls, &time_passed);
                    

                    std::cout << num_of_iterations << ", " << num_of_terms << ", " << num_of_zeroes << ", " << sp.f(best_input) << ", " << sp.f(worst_input) << ", " << sp.f(random_solution) << std::endl;

                    CMA_ES_Solver cma_es2(50, 1, 0.5, num_of_iterations);
                    sp.resetCounter();
                    double *cma_es_solution = cma_es2.solve(&sp, 5406, &best_guess, &num_of_f_calls, &time_passed);
                    std::cout << "CMA ES Solution" << std::endl;

                    std::cout << "iteration,Best_Guess,Function_Calls,Time_Passed" << std::endl;
                    for (int i = 0; i < best_guess.size(); i++) {
                        std::cout << i << ", " << (double) best_guess[i] << ", " << num_of_f_calls[i] << ", " << time_passed[i] << std::endl;
                    }

                    best_guess.clear();
                    num_of_f_calls.clear();
                    time_passed.clear();

                    Nelder_Mead_Solver nms2(num_of_terms * 4 + 7, num_of_iterations);
                    sp.resetCounter();
                    double *nms_solution = nms2.solve(&sp, rep_index * 7, &best_guess, &num_of_f_calls, &time_passed);
                    std::cout << "Nelder Mead Solution" << std::endl;
                    
                    std::cout << "iteration,Best_Guess,Function_Calls,Time_Passed" << std::endl;
                    for (int i = 0; i < best_guess.size(); i++) {
                        std::cout << i << ", " << (double) best_guess[i] << ", " << num_of_f_calls[i] << ", " << time_passed[i] << std::endl;
                    }

                    best_guess.clear();
                    num_of_f_calls.clear();
                    time_passed.clear();

                    PSO_Solver pso2(num_of_terms * 4 + 1, num_of_iterations, 0.5, 2, 2);
                    sp.resetCounter();
                    double *pso_solution = pso2.solve(&sp, (unsigned) time(NULL), &best_guess, &num_of_f_calls, &time_passed);
                    std::cout << "PSO Solution" << std::endl;

                    std::cout << "iteration,Best_Guess,Function_Calls,Time_Passed" << std::endl;
                    for (int i = 0; i < best_guess.size(); i++) {
                        std::cout << i << ", " << (double) best_guess[i] << ", " << num_of_f_calls[i] << ", " << time_passed[i] << std::endl;
                    }

                    best_guess.clear();
                    num_of_f_calls.clear();
                    time_passed.clear();

                    delete [] random_solution;
                    delete [] cma_es_solution;
                    delete [] nms_solution;
                    delete [] pso_solution;
                } catch(int errorNum) {

                }
            }
        }
    }

    return 0;
}