#include "opt_solver.h"

class Nelder_Mead_Solver : public OptSolver {
    public:
        double* solve(OptProblem *optProblem, uint randomSeed) override {

            // Simple Nelder-Mead Simplex Algorithm

            double **simplex_vertices = optProblem->generateRandomFeasibleInputs(3, randomSeed);
            for (int i = 0; i < 300; i++) {
                    // Sort the simplex by the function values
                    double function_evals[3];
                    uint vertices_order[3];

                    std::cout << "\nTesting index: " << i << std::endl; 
                    for (uint j = 0; j < 3; j++) {
                        function_evals[j] = optProblem->f(simplex_vertices[j]);
                        vertices_order[j] = j;
                    }

                    // std::cout << "Testing bubble sort" << std::endl; 
                    for (int k = 0; k < 3 - 1; k++) {
                        for (int l = 0; l < 3 - k - 1; l++) {
                            if (function_evals[vertices_order[l]] > function_evals[vertices_order[l + 1]]) {
                                // Swap arr[l] and arr[l + 1]
                                int temp = vertices_order[l];
                                vertices_order[l] = vertices_order[l + 1];
                                vertices_order[l + 1] = temp;
                            }
                        }
                    }

                    std::cout << "Simplex Vertices are (" << simplex_vertices[vertices_order[0]][0] << ", " << simplex_vertices[vertices_order[0]][1] << "), (" << simplex_vertices[vertices_order[1]][0] << ", " << simplex_vertices[vertices_order[1]][1] << "), (" << simplex_vertices[vertices_order[2]][0] << ", " << simplex_vertices[vertices_order[2]][1] << ")" << std::endl;
                    std::cout << "Best input: (" << simplex_vertices[vertices_order[0]][0] << ", " << simplex_vertices[vertices_order[0]][1] << ") => " << function_evals[vertices_order[0]] << std::endl;


                    // Calculate the centroid of the best vertices
                    double best_centroid[2];
                    best_centroid[0] = (simplex_vertices[vertices_order[0]][0] + simplex_vertices[vertices_order[1]][0]) / 2;
                    best_centroid[1] = (simplex_vertices[vertices_order[0]][1] + simplex_vertices[vertices_order[1]][1]) / 2;

                    // Reflect
                    double new_point[2];
                    new_point[0] = best_centroid[0] + (best_centroid[0] - simplex_vertices[vertices_order[2]][0]);
                    new_point[1] = best_centroid[1] + (best_centroid[1] - simplex_vertices[vertices_order[2]][1]);

                    double new_point_eval = optProblem->f(new_point);

                    std::cout << "New Point: (" << new_point[0] << ", " << new_point[1] << ") => " << new_point_eval << std::endl;
                    std::cout << "New Point deviation: (" << best_centroid[0] - simplex_vertices[vertices_order[2]][0] << ", " << best_centroid[1] - simplex_vertices[vertices_order[2]][1] << ") => " << new_point_eval << std::endl;
                    // std::cout << "Testing new_point: (" << new_point[0] << ", " << new_point[1] << ")." << std::endl;
                    // std::cout << "Testing new_point_eval: (" << new_point_eval << ")." << std::endl;

                    // Expand
                    if (new_point_eval <= function_evals[vertices_order[0]]) {
                        double *greedy_point = new double[2];
                        greedy_point[0] = best_centroid[0] + 2 * (best_centroid[0] - simplex_vertices[vertices_order[2]][0]);
                        greedy_point[1] = best_centroid[1] + 2 * (best_centroid[1] - simplex_vertices[vertices_order[2]][1]);

                        double greedy_point_eval = optProblem->f(greedy_point);

                        if (greedy_point_eval <= new_point_eval) {
                            simplex_vertices[vertices_order[2]] = greedy_point;
                            std::cout << "GREED" << std::endl;
                        } else {
                            simplex_vertices[vertices_order[2]] = new_point;
                            std::cout << "EXPAND" << std::endl;
                        }

                        continue;
                    }

                    // Contract outside
                    double *contract_point[2];
                    contract_point[0] = new double[2];
                    contract_point[1] = new double[2];

                    contract_point[0][0] = best_centroid[0] - 0.5 * (best_centroid[0] - simplex_vertices[vertices_order[2]][0]);
                    contract_point[0][1] = best_centroid[1] - 0.5 * (best_centroid[1] - simplex_vertices[vertices_order[2]][1]);

                    contract_point[1][0] = best_centroid[0] + 0.5 * (best_centroid[0] - simplex_vertices[vertices_order[2]][0]);
                    contract_point[1][1] = best_centroid[1] + 0.5 * (best_centroid[1] - simplex_vertices[vertices_order[2]][1]);

                    double *best_contract_point = contract_point[0];
                    double best_contract_point_eval = optProblem->f(contract_point[0]);

                    double other_contract_point_eval = optProblem->f(contract_point[1]);

                    if (other_contract_point_eval < best_contract_point_eval) {
                        best_contract_point = contract_point[1];
                        best_contract_point_eval = other_contract_point_eval;
                    }

                    if (best_contract_point_eval <= function_evals[vertices_order[1]]) {
                        simplex_vertices[vertices_order[2]] = best_contract_point;

                        std::cout << "CONTRACT" << std::endl;

                        continue;
                    }

                    // Shrink
                    std::cout << "Before shrinking vertices are (" << simplex_vertices[vertices_order[0]][0] << ", " << simplex_vertices[vertices_order[0]][1] << "), (" << simplex_vertices[vertices_order[1]][0] << ", " << simplex_vertices[vertices_order[1]][1] << "), (" << simplex_vertices[vertices_order[2]][0] << ", " << simplex_vertices[vertices_order[2]][1] << ")" << std::endl;
                    double *shrink_1 = new double[2];
                    double *shrink_2 = new double[2];
                    
                    shrink_1[0] = (simplex_vertices[vertices_order[1]][0] + simplex_vertices[vertices_order[0]][0]) / 2;
                    shrink_1[1] = (simplex_vertices[vertices_order[1]][1] + simplex_vertices[vertices_order[0]][1]) / 2;

                    shrink_2[0] = (simplex_vertices[vertices_order[2]][0] + simplex_vertices[vertices_order[0]][0]) / 2;
                    shrink_2[1] = (simplex_vertices[vertices_order[2]][1] + simplex_vertices[vertices_order[0]][1]) / 2;

                    simplex_vertices[vertices_order[1]] = shrink_1;
                    simplex_vertices[vertices_order[2]] = shrink_2;

                    std::cout << "Shrinking Vertices are (" << simplex_vertices[vertices_order[0]][0] << ", " << simplex_vertices[vertices_order[0]][1] << "), (" << simplex_vertices[vertices_order[1]][0] << ", " << simplex_vertices[vertices_order[1]][1] << "), (" << simplex_vertices[vertices_order[2]][0] << ", " << simplex_vertices[vertices_order[2]][1] << ")" << std::endl;
                    std::cout << "SHRINK" << std::endl;
            }

            return simplex_vertices[0];
        }
};