#include <iostream>
#include <string>
#include <random>
#include <cstdlib>

#include "main.h"

class FLP : public OptProblem {
    public:

        FLP(uint numDemand, uint numFacilities, uint *facilityCapacities, uint *facilityStartupCost, uint *demandNodeDemand, uint *variableCosts) 
            : OptProblem(OptimizationType::DISCRETE)
            , num_of_demand_nodes(numDemand), num_of_potential_facilities(numFacilities)
            , facility_capacities(facilityCapacities), facility_startup_cost(facilityStartupCost)
            , demand_node_demand(demandNodeDemand), variable_costs(variableCosts) {}

        // Implementation of the pure virtual function f() for this problem
        double f(double *input) override {
            
            // Checking solution

            // Input set up
            double* activated_facilities = input;
            double* assigned_customers = (input + num_of_potential_facilities);

            for (uint demand_index = 0; demand_index < num_of_demand_nodes; demand_index++) {
                for (uint fac_index = 0; fac_index < num_of_potential_facilities; fac_index++) {
                    assigned_customers[(demand_index * num_of_potential_facilities) + fac_index] = ((demand_index + fac_index) % 4) * 7;
                }
            }

            uint total_cost = 0;
            // Adding start up cost
            for (uint i = 0; i < num_of_potential_facilities; i++) {
                total_cost += (activated_facilities[i] * facility_startup_cost[i]);
            }
            // Adding variable cost
            for (uint demand_index = 0; demand_index < num_of_demand_nodes; demand_index++) {
                for (uint fac_index = 0; fac_index < num_of_potential_facilities; fac_index++) {
                    total_cost += activated_facilities[fac_index] * assigned_customers[(demand_index * num_of_potential_facilities) + fac_index] * variable_costs[(demand_index * num_of_potential_facilities) + fac_index];
                }
            }

            std::cout << "\nThe total cost of the inputs: " << total_cost << std::endl;

            // Checking constraints

            // Checking customer constraint
            uint customers_num = 0;
            uint customers_serviced = 0;
            for (uint i = 0; i < num_of_demand_nodes; i++) {
                customers_num += demand_node_demand[i];
            }
            for (uint demand_index = 0; demand_index < num_of_demand_nodes; demand_index++) {
                for (uint fac_index = 0; fac_index < num_of_potential_facilities; fac_index++) {
                    customers_serviced += assigned_customers[(demand_index * num_of_potential_facilities) + fac_index] * activated_facilities[fac_index];
                }
            }
            std::cout << "Serviced " << customers_serviced << "/" << customers_num << " customers." << std::endl;

            // Checking facility capacity constraint
            for (uint fac_index = 0; fac_index < num_of_potential_facilities; fac_index++) {
                uint facility_customer_serviced = 0;

                for (uint demand_index = 0; demand_index < num_of_demand_nodes; demand_index++) {
                    facility_customer_serviced += assigned_customers[(demand_index * num_of_potential_facilities) + fac_index] * activated_facilities[fac_index];
                }
                std::cout << "Facility " << fac_index << " services " << facility_customer_serviced << "/" << facility_capacities[fac_index] << " customers." << std::endl;
            }
            
            return total_cost;
        }

        uint getInputDimension() override { return num_of_potential_facilities + num_of_potential_facilities * num_of_demand_nodes; };
        double* getInputConstraints() override { 
            return new double[4]; 
        };

        double** generateRandomFeasibleInputs(uint num_of_inputs, uint randomSeed) override { return new double*[2]; };

    private:
        uint num_of_demand_nodes, num_of_potential_facilities;
        uint *facility_capacities, *facility_startup_cost, *demand_node_demand, *variable_costs;

};

// Optimal solution is (0, -2.67298)
class UncapacitatedRandomEquation : public OptProblem {
        public:
            UncapacitatedRandomEquation() 
            : OptProblem(OptimizationType::CONTINUOUS) {}

            double f(double *input) override {
                double x = input[0];
                double y = input[1];

                return (x*x - 5) + (y * y * y * y + 4 * y * y * y + y * y - 4 * y) + (x * x * y * y);
            };
            uint getInputDimension() override { return 2; };
            double* getInputConstraints() override { 
                // Do not trust this. I should malloc
                double constraints[2] = { 9999.0, 9999.0 };

                return constraints; 
            };


            double** generateRandomFeasibleInputs(uint num_of_inputs, uint randomSeed) override { 
                double **feasible_inputs = new double*[2]; 
                uint input_dimension = getInputDimension();

                srand(randomSeed); 

                for (uint i = 0; i < num_of_inputs; i++) {
                    feasible_inputs[i] = new double[input_dimension];

                    for (uint j = 0; j < input_dimension; j++) {
                        feasible_inputs[i][j] = (rand() % 200) - 100;
                    }
                }

                return feasible_inputs;
            };
};

int main() {

    // Problem Set Up
    uint num_of_demand_nodes, num_of_potential_facilities;

    num_of_demand_nodes = 10;
    num_of_potential_facilities = 5;

    uint* facility_capacities = new uint[num_of_potential_facilities];
    uint* facility_startup_cost = new uint[num_of_potential_facilities];

    for (uint i = 0; i < num_of_potential_facilities; i++) {
        facility_capacities[i] = (i + 1) * 100;
        facility_startup_cost[i] = (num_of_potential_facilities - i) * 100;
    }

    uint* demand_node_demand = new uint[num_of_demand_nodes];
    for (uint i = 0; i < num_of_demand_nodes; i++) {
        demand_node_demand[i] = (i + 1) * 27;
    }

    uint* variable_costs = new uint[num_of_demand_nodes * num_of_potential_facilities];
    for (uint demand_index = 0; demand_index < num_of_demand_nodes; demand_index++) {
        for (uint fac_index = 0; fac_index < num_of_potential_facilities; fac_index++) {
            variable_costs[(demand_index * num_of_potential_facilities) + fac_index] = ((demand_index + fac_index) % 3) + 1;
        }
    }

    // Problem text debugging
    std::cout << "The number of demand nodes is: " << num_of_demand_nodes << std::endl;
    std::cout << "The number of potential facilities is: " << num_of_potential_facilities << std::endl;
    
    std::string facility_capacity_text = "The capacities of each facility is ";
    for (uint i = 0; i < num_of_potential_facilities; i++) {
        facility_capacity_text += std::to_string(facility_capacities[i]) + ", ";
    }
    std::cout <<  facility_capacity_text << std::endl;

    std::string facility_startup_cost_text = "The start up cost of each facility is ";
    for (uint i = 0; i < num_of_potential_facilities; i++) {
        facility_startup_cost_text += std::to_string(facility_startup_cost[i]) + ", ";
    }
    std::cout <<  facility_startup_cost_text << std::endl;

    // Checking solution

    // Input set up
    bool* activated_facilities = new bool[num_of_potential_facilities];
    for (uint i = 0; i < num_of_potential_facilities; i++) {
        activated_facilities[i] = (i + 1) % 2;
    }

    uint* assigned_customers = new uint[num_of_demand_nodes * num_of_potential_facilities];
    for (uint demand_index = 0; demand_index < num_of_demand_nodes; demand_index++) {
        for (uint fac_index = 0; fac_index < num_of_potential_facilities; fac_index++) {
            assigned_customers[(demand_index * num_of_potential_facilities) + fac_index] = ((demand_index + fac_index) % 4) * 7;
        }
    }

    //Testing
    FLP flp = FLP(num_of_demand_nodes, num_of_potential_facilities, facility_capacities, facility_startup_cost, demand_node_demand, variable_costs);

    double *input = new double[num_of_potential_facilities + num_of_demand_nodes * num_of_potential_facilities];

    for (uint i = 0; i < num_of_potential_facilities; i++) {
        input[i] = activated_facilities[i];
    }

    for (uint i = 0; i < num_of_demand_nodes * num_of_potential_facilities; i++) {
        input[i + num_of_potential_facilities] = assigned_customers[i];
    }

    flp.f(input);

    // CFLP too difficult as of rn
    UncapacitatedRandomEquation ure = UncapacitatedRandomEquation();

    double ure_input[2] = { 0, -2.7928 };
    double ure_result = ure.f(ure_input);

    std::cout << "\nUncapacitatedRandomEquation minimum value is: " << ure_result << std::endl;

    double **feasible_inputs = ure.generateRandomFeasibleInputs(3, (unsigned) time(NULL));
    for (uint i = 0; i < 3; i++) {
        std::cout << "Feasible_input: (" << feasible_inputs[i][0] << ", " << feasible_inputs[i][1] << ")." << std::endl;
    }

    // Simple Nelder-Mead Simplex Algorithm

    double **simplex_vertices = ure.generateRandomFeasibleInputs(3, (unsigned) time(NULL));
    for (int i = 0; i < 300; i++) {
            // Sort the simplex by the function values
            double function_evals[3];
            uint vertices_order[3];

            std::cout << "\nTesting index: " << i << std::endl; 
            for (uint j = 0; j < 3; j++) {
                function_evals[j] = ure.f(simplex_vertices[j]);
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

            double new_point_eval = ure.f(new_point);

            std::cout << "New Point: (" << new_point[0] << ", " << new_point[1] << ") => " << new_point_eval << std::endl;
            std::cout << "New Point deviation: (" << best_centroid[0] - simplex_vertices[vertices_order[2]][0] << ", " << best_centroid[1] - simplex_vertices[vertices_order[2]][1] << ") => " << new_point_eval << std::endl;
            // std::cout << "Testing new_point: (" << new_point[0] << ", " << new_point[1] << ")." << std::endl;
            // std::cout << "Testing new_point_eval: (" << new_point_eval << ")." << std::endl;

            // Expand
            if (new_point_eval <= function_evals[vertices_order[0]]) {
                double *greedy_point = new double[2];
                greedy_point[0] = best_centroid[0] + 2 * (best_centroid[0] - simplex_vertices[vertices_order[2]][0]);
                greedy_point[1] = best_centroid[1] + 2 * (best_centroid[1] - simplex_vertices[vertices_order[2]][1]);

                double greedy_point_eval = ure.f(greedy_point);

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
            double best_contract_point_eval = ure.f(contract_point[0]);

            double other_contract_point_eval = ure.f(contract_point[1]);

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

            // Termination condition
            // ...
    }

    return 0;
}