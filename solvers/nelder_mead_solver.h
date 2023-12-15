#include "opt_solver.h"

class Nelder_Mead_Solver : public OptSolver {
    public:
        Nelder_Mead_Solver(int num_of_vertices, uint num_of_iterations) 
            : OptSolver() {
                this->num_of_vertices = num_of_vertices;
                this->num_of_iterations = num_of_iterations;
        }

        double* solve(OptProblem *optProblem, uint randomSeed, std::vector<double> *best_answer, std::vector<int> *num_of_f_calls, std::vector<int> *num_of_time_passed) override {

            // Record Time
            auto started = std::chrono::high_resolution_clock::now();

            // Simple Nelder-Mead Simplex Algorithm
            uint input_dimension = optProblem->getInputDimension();

            double **simplex_vertices = optProblem->generateRandomFeasibleInputs(this->num_of_vertices, randomSeed);
            double function_evals[this->num_of_vertices];
            uint vertices_order[this->num_of_vertices];

            for (int i = 0; i < this->num_of_iterations; i++) {
                    // Sort the simplex by the function values

                    for (uint j = 0; j < this->num_of_vertices; j++) {
                        function_evals[j] = optProblem->f(simplex_vertices[j]);
                        vertices_order[j] = j;
                    }

                    for (int k = 0; k < this->num_of_vertices - 1; k++) {
                        for (int l = 0; l < this->num_of_vertices - k - 1; l++) {
                            if (function_evals[vertices_order[l]] > function_evals[vertices_order[l + 1]]) {
                                // Swap arr[l] and arr[l + 1]
                                int temp = vertices_order[l];
                                vertices_order[l] = vertices_order[l + 1];
                                vertices_order[l + 1] = temp;
                            }
                        }
                    }

                    // Push to info
                    best_answer->push_back(function_evals[vertices_order[0]]);
                    num_of_f_calls->push_back(optProblem->getNumCalled());

                    // Timer
                    auto done = std::chrono::high_resolution_clock::now();
                    num_of_time_passed->push_back(std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count());

                    // Calculate the centroid of the best vertices
                    double best_centroid[input_dimension] = {0};
                    for (int curr_dimension =0; curr_dimension < input_dimension; curr_dimension++) {
                        for (int vertex_index = 0; vertex_index < this->num_of_vertices - 1; vertex_index++) {
                            best_centroid[curr_dimension] += simplex_vertices[vertices_order[vertex_index]][curr_dimension];
                        }

                        best_centroid[curr_dimension] = best_centroid[curr_dimension] / (this->num_of_vertices - 1);
                    }

                    // Reflect
                    double new_point[input_dimension];
                    for (int curr_dimension =0; curr_dimension < input_dimension; curr_dimension++) {
                        new_point[curr_dimension] = best_centroid[curr_dimension] + (best_centroid[curr_dimension] - simplex_vertices[vertices_order[this->num_of_vertices - 1]][curr_dimension]);
                    }

                    double new_point_eval = optProblem->f(new_point);

                    // Expand
                    if (new_point_eval <= function_evals[vertices_order[0]]) {
                        double *greedy_point = new double[input_dimension];

                        for (int curr_dimension =0; curr_dimension < input_dimension; curr_dimension++) {
                            greedy_point[curr_dimension] = best_centroid[curr_dimension] + 2 * (best_centroid[curr_dimension] - simplex_vertices[vertices_order[this->num_of_vertices - 1]][curr_dimension]);
                        }

                        double greedy_point_eval = optProblem->f(greedy_point);

                        if (greedy_point_eval <= new_point_eval) {
                            simplex_vertices[vertices_order[2]] = greedy_point;
                            // std::cout << "GREED" << std::endl;
                        } else {
                            simplex_vertices[vertices_order[2]] = new_point;
                            // std::cout << "EXPAND" << std::endl;
                        }

                        continue;
                    }

                    // Contract outside
                    double *contract_point[2];
                    contract_point[0] = new double[input_dimension];
                    contract_point[1] = new double[input_dimension];

                    for (int curr_dimension =0; curr_dimension < input_dimension; curr_dimension++) {
                        contract_point[0][curr_dimension] = best_centroid[curr_dimension] - 0.5 * (best_centroid[curr_dimension] - simplex_vertices[vertices_order[this->num_of_vertices - 1]][curr_dimension]);
                        contract_point[1][curr_dimension] = best_centroid[curr_dimension] + 0.5 * (best_centroid[curr_dimension] - simplex_vertices[vertices_order[this->num_of_vertices - 1]][curr_dimension]);
                    } 

                    double *best_contract_point = contract_point[0];
                    double best_contract_point_eval = optProblem->f(contract_point[0]);

                    double other_contract_point_eval = optProblem->f(contract_point[1]);

                    if (other_contract_point_eval < best_contract_point_eval) {
                        best_contract_point = contract_point[1];
                        best_contract_point_eval = other_contract_point_eval;
                    }

                    if (best_contract_point_eval <= function_evals[vertices_order[1]]) {
                        simplex_vertices[vertices_order[2]] = best_contract_point;

                        // std::cout << "CONTRACT" << std::endl;

                        continue;
                    }

                    // Shrink
                    double **shrinks = new double*[this->num_of_vertices - 1];

                    for (int curr_vertex = 1; curr_vertex < this->num_of_vertices; curr_vertex++) {
                        shrinks[curr_vertex - 1] = new double[input_dimension];

                        for (int curr_dimension; curr_dimension < input_dimension; curr_dimension++) {
                            shrinks[curr_vertex - 1][curr_dimension] = (simplex_vertices[vertices_order[curr_vertex]][curr_dimension] + simplex_vertices[vertices_order[0]][curr_dimension]) / 2;
                        }
                    }
                    
                    for (int curr_vertex = 1; curr_vertex < this->num_of_vertices; curr_vertex++) {
                        simplex_vertices[vertices_order[curr_vertex]] = shrinks[curr_vertex - 1];
                    }

                    
            }

            // std::cout << "Solved" << std::endl;
            
            double *best_solution = new double[input_dimension];

            for (int i = 0; i < input_dimension; i++) {
                best_solution[i] = simplex_vertices[vertices_order[0]][i];
            }

            return best_solution;
        }

    private:
        uint num_of_vertices;
        uint num_of_iterations;
};