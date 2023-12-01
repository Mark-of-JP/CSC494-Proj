#include "opt_solver.h"

class Nelder_Mead_Solver : public OptSolver {
    public:
        Nelder_Mead_Solver(int num_of_vertices) 
            : OptSolver() {
                this->num_of_vertices = num_of_vertices;
        }

        double* solve(OptProblem *optProblem, uint randomSeed) override {

            // Simple Nelder-Mead Simplex Algorithm
            uint input_dimension = optProblem->getInputDimension();

            std::cout << "START" << std::endl;

            double **simplex_vertices = optProblem->generateRandomFeasibleInputs(this->num_of_vertices, randomSeed);
            double function_evals[this->num_of_vertices];
            uint vertices_order[this->num_of_vertices];

            std::cout << "Generated" << std::endl;
            for (int i = 0; i < 300; i++) {
                    // Sort the simplex by the function values

                    // std::cout << "\nTesting index: " << i << std::endl; 
                    for (uint j = 0; j < this->num_of_vertices; j++) {
                        function_evals[j] = optProblem->f(simplex_vertices[j]);
                        vertices_order[j] = j;
                    }

                    // std::cout << "Testing bubble sort" << std::endl; 
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

                    // std::cout << "Simplex Vertices are (" << simplex_vertices[vertices_order[0]][0] << ", " << simplex_vertices[vertices_order[0]][1] << "), (" << simplex_vertices[vertices_order[1]][0] << ", " << simplex_vertices[vertices_order[1]][1] << "), (" << simplex_vertices[vertices_order[2]][0] << ", " << simplex_vertices[vertices_order[2]][1] << ")" << std::endl;
                    // std::cout << "Best input: (" << simplex_vertices[vertices_order[0]][0] << ", " << simplex_vertices[vertices_order[0]][1] << ") => " << function_evals[vertices_order[0]] << std::endl;


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

                    // std::cout << "New Point: (" << new_point[0] << ", " << new_point[1] << ") => " << new_point_eval << std::endl;
                    // std::cout << "New Point deviation: (" << best_centroid[0] - simplex_vertices[vertices_order[2]][0] << ", " << best_centroid[1] - simplex_vertices[vertices_order[2]][1] << ") => " << new_point_eval << std::endl;
                    // std::cout << "Testing new_point: (" << new_point[0] << ", " << new_point[1] << ")." << std::endl;
                    // std::cout << "Testing new_point_eval: (" << new_point_eval << ")." << std::endl;

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
                    // std::cout << "Before shrinking vertices are (" << simplex_vertices[vertices_order[0]][0] << ", " << simplex_vertices[vertices_order[0]][1] << "), (" << simplex_vertices[vertices_order[1]][0] << ", " << simplex_vertices[vertices_order[1]][1] << "), (" << simplex_vertices[vertices_order[2]][0] << ", " << simplex_vertices[vertices_order[2]][1] << ")" << std::endl;
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

                    // std::cout << "Shrinking Vertices are (" << simplex_vertices[vertices_order[0]][0] << ", " << simplex_vertices[vertices_order[0]][1] << "), (" << simplex_vertices[vertices_order[1]][0] << ", " << simplex_vertices[vertices_order[1]][1] << "), (" << simplex_vertices[vertices_order[2]][0] << ", " << simplex_vertices[vertices_order[2]][1] << ")" << std::endl;
                    // std::cout << "SHRINK" << std::endl;
            }

            std::cout << "Solved" << std::endl;
            
            double *best_solution = new double[input_dimension];

            for (int i = 0; i < input_dimension; i++) {
                best_solution[i] = simplex_vertices[vertices_order[0]][i];
            }

            return best_solution;
        }

    private:
        uint num_of_vertices;
};