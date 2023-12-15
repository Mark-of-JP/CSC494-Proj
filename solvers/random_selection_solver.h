#include "opt_solver.h"

class RandomSelection : public OptSolver {
    public:
        RandomSelection(int num_of_selections) 
            : OptSolver() {
                this->num_of_selections = num_of_selections;
        }

        double* solve(OptProblem *optProblem, uint randomSeed, std::vector<double> *best_answer, std::vector<int> *num_of_f_calls, std::vector<int> *num_of_time_passed) override {

            // Simple Nelder-Mead Simplex Algorithm
            uint input_dimension = optProblem->getInputDimension();

            double **random_solutions = optProblem->generateRandomFeasibleInputs(this->num_of_selections, randomSeed);
            
            int best_solution_index = 0;
            double best_solution_value = optProblem->f(random_solutions[0], false);

            for (int i = 1; i < this->num_of_selections; i++) {
                double curr_solution_value = optProblem->f(random_solutions[i], false);

                if (curr_solution_value < best_solution_value) {
                    best_solution_value = curr_solution_value;
                    best_solution_index = i;
                }
            }

            // std::cout << "Solved" << std::endl;
            
            double *best_solution = new double[input_dimension];

            for (int i = 0; i < input_dimension; i++) {
                best_solution[i] = random_solutions[best_solution_index][i];
            }

            return best_solution;
        }

    private:
        uint num_of_selections;
};