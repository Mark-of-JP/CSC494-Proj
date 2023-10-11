#include <iostream>
#include <string>

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
    uint* customers_assigned_to_facilities = new uint[num_of_demand_nodes * num_of_potential_facilities];

    for (uint i = 0; i < num_of_potential_facilities; i++) {
        activated_facilities[i] = i % 2;
    }

    uint* assigned_customers = new uint[num_of_demand_nodes * num_of_potential_facilities];
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
            customers_serviced += assigned_customers[(demand_index * num_of_potential_facilities) + fac_index];
        }
    }
    std::cout << "Serviced " << customers_serviced << "/" << customers_num << " customers." << std::endl;

    // Checking facility capacity constraint
    for (uint fac_index = 0; fac_index < num_of_potential_facilities; fac_index++) {
        uint facility_customer_serviced = 0;

        for (uint demand_index = 0; demand_index < num_of_demand_nodes; demand_index++) {
            facility_customer_serviced += assigned_customers[(demand_index * num_of_potential_facilities) + fac_index];
        }
        std::cout << "Facility " << fac_index << " services " << facility_customer_serviced << "/" << facility_capacities[fac_index] << " customers." << std::endl;
    }

    return 0;
}