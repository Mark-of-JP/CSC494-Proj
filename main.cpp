#include <iostream>
#include <string>
#include <random>
#include <cstdlib>

#include "main.h"

// class FLP : public OptProblem {
//     public:

//         FLP(uint numDemand, uint numFacilities, uint *facilityCapacities, uint *facilityStartupCost, uint *demandNodeDemand, uint *variableCosts) 
//             : OptProblem(OptimizationType::DISCRETE)
//             , num_of_demand_nodes(numDemand), num_of_potential_facilities(numFacilities)
//             , facility_capacities(facilityCapacities), facility_startup_cost(facilityStartupCost)
//             , demand_node_demand(demandNodeDemand), variable_costs(variableCosts) {}

//         // Implementation of the pure virtual function f() for this problem
//         double f_implement(double *input) override {
            
//             // Checking solution

//             // Input set up
//             double* activated_facilities = input;
//             double* assigned_customers = (input + num_of_potential_facilities);

//             for (uint demand_index = 0; demand_index < num_of_demand_nodes; demand_index++) {
//                 for (uint fac_index = 0; fac_index < num_of_potential_facilities; fac_index++) {
//                     assigned_customers[(demand_index * num_of_potential_facilities) + fac_index] = ((demand_index + fac_index) % 4) * 7;
//                 }
//             }

//             uint total_cost = 0;
//             // Adding start up cost
//             for (uint i = 0; i < num_of_potential_facilities; i++) {
//                 total_cost += (activated_facilities[i] * facility_startup_cost[i]);
//             }
//             // Adding variable cost
//             for (uint demand_index = 0; demand_index < num_of_demand_nodes; demand_index++) {
//                 for (uint fac_index = 0; fac_index < num_of_potential_facilities; fac_index++) {
//                     total_cost += activated_facilities[fac_index] * assigned_customers[(demand_index * num_of_potential_facilities) + fac_index] * variable_costs[(demand_index * num_of_potential_facilities) + fac_index];
//                 }
//             }

//             std::cout << "\nThe total cost of the inputs: " << total_cost << std::endl;

//             // Checking constraints

//             // Checking customer constraint
//             uint customers_num = 0;
//             uint customers_serviced = 0;
//             for (uint i = 0; i < num_of_demand_nodes; i++) {
//                 customers_num += demand_node_demand[i];
//             }
//             for (uint demand_index = 0; demand_index < num_of_demand_nodes; demand_index++) {
//                 for (uint fac_index = 0; fac_index < num_of_potential_facilities; fac_index++) {
//                     customers_serviced += assigned_customers[(demand_index * num_of_potential_facilities) + fac_index] * activated_facilities[fac_index];
//                 }
//             }
//             std::cout << "Serviced " << customers_serviced << "/" << customers_num << " customers." << std::endl;

//             // Checking facility capacity constraint
//             for (uint fac_index = 0; fac_index < num_of_potential_facilities; fac_index++) {
//                 uint facility_customer_serviced = 0;

//                 for (uint demand_index = 0; demand_index < num_of_demand_nodes; demand_index++) {
//                     facility_customer_serviced += assigned_customers[(demand_index * num_of_potential_facilities) + fac_index] * activated_facilities[fac_index];
//                 }
//                 std::cout << "Facility " << fac_index << " services " << facility_customer_serviced << "/" << facility_capacities[fac_index] << " customers." << std::endl;
//             }
            
//             return total_cost;
//         }

//         uint getInputDimension() override { return num_of_potential_facilities + num_of_potential_facilities * num_of_demand_nodes; };
//         double* getInputConstraints() override { 
//             return new double[4]; 
//         };

//         double** generateRandomFeasibleInputs(uint num_of_inputs, uint randomSeed) override { return new double*[2]; };

//     private:
//         uint num_of_demand_nodes, num_of_potential_facilities;
//         uint *facility_capacities, *facility_startup_cost, *demand_node_demand, *variable_costs;

// };

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
    // FLP flp = FLP(num_of_demand_nodes, num_of_potential_facilities, facility_capacities, facility_startup_cost, demand_node_demand, variable_costs);

    // double *input = new double[num_of_potential_facilities + num_of_demand_nodes * num_of_potential_facilities];

    // for (uint i = 0; i < num_of_potential_facilities; i++) {
    //     input[i] = activated_facilities[i];
    // }

    // for (uint i = 0; i < num_of_demand_nodes * num_of_potential_facilities; i++) {
    //     input[i + num_of_potential_facilities] = assigned_customers[i];
    // }

    // flp.f(input);

    // CFLP too difficult as of rn
    UncapacitatedRandomEquation ure = UncapacitatedRandomEquation();

    double ure_input[2] = { 0, -2.7928 };
    double ure_result = ure.f(ure_input, false);

    std::cout << "\nUncapacitatedRandomEquation minimum value is: " << ure_result << std::endl;

    Nelder_Mead_Solver nms;

    nms.solve(&ure, (unsigned) time(NULL));

    return 0;
}