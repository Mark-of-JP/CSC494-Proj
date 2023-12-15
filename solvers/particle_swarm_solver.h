#include "opt_solver.h"

class PSO_Solver : public OptSolver {
    public:

        PSO_Solver(int num_of_particles, int num_of_iterations, double inertia_weight, double r_1_weight, double r_2_weight) 
            : OptSolver() {
                this->num_of_particles = num_of_particles;
                this->num_of_iterations = num_of_iterations;

                this->inertia_weight = inertia_weight;
                this->r_1_weight = r_1_weight;
                this->r_2_weight = r_2_weight;
        }

        double* solve(OptProblem *optProblem, uint randomSeed, std::vector<double> *best_answer, std::vector<int> *num_of_f_calls, std::vector<int> *num_of_time_passed) override {

            // Record Time
            auto started = std::chrono::high_resolution_clock::now();

            // Initialize all the variables
            int input_dimension = optProblem->getInputDimension();

            double** particle_position = optProblem->generateRandomFeasibleInputs(this->num_of_particles, randomSeed);

            double particle_velocity[this->num_of_particles][input_dimension];
            
            double particle_solo_best_position[this->num_of_particles][input_dimension];
            double particle_solo_best_value[this->num_of_particles];

            int best_particle_index = 0;

            srand(randomSeed);

            // Set initial values
            for (int particle_index = 0; particle_index < this->num_of_particles; particle_index++) {
                for (int p_dimension = 0; p_dimension < input_dimension; p_dimension++) {
                    //Setting initial velocity to 0
                    particle_velocity[particle_index][p_dimension] = 0;

                    // Setting best values to current values
                    particle_solo_best_position[particle_index][p_dimension] = particle_position[particle_index][p_dimension];
                }

                double curr_value = optProblem->f(particle_solo_best_position[particle_index]);

                particle_solo_best_value[particle_index] = curr_value;

                if (curr_value < particle_solo_best_value[particle_index]) {
                    best_particle_index = particle_index;
                }
            }

            for (int curr_iteration = 0; curr_iteration < this->num_of_iterations; curr_iteration++) {

                // Create new velocity
                for (int p_dimension = 0; p_dimension < input_dimension; p_dimension++) {

                    double r_1 = (float) rand() / RAND_MAX;
                    double r_2 = (float) rand() / RAND_MAX;

                    for (int particle_index = 0; particle_index < this->num_of_particles; particle_index++) {

                        particle_velocity[particle_index][p_dimension] = this->inertia_weight * particle_velocity[particle_index][p_dimension] 
                            + this->r_1_weight * r_1 * (particle_solo_best_position[particle_index][p_dimension] - particle_position[particle_index][p_dimension])
                            + this->r_2_weight * r_2 * (particle_solo_best_position[best_particle_index][p_dimension] - particle_position[particle_index][p_dimension]);

                    }
                }

                for (int particle_index = 0; particle_index < this->num_of_particles; particle_index++) {
                    
                    // If not best particle then move
                    if (particle_index != best_particle_index) {
                        for (int p_dimension = 0; p_dimension < input_dimension; p_dimension++) {
                            particle_position[particle_index][p_dimension] = particle_position[particle_index][p_dimension] + particle_velocity[particle_index][p_dimension];
                        }
                    }
                }

                for (int particle_index = 0; particle_index < this->num_of_particles; particle_index++) {

                    // Update new bests
                    double new_value = optProblem->f(particle_position[particle_index]);

                    // If global minimum then set global minimum
                    if (new_value < particle_solo_best_value[best_particle_index]) {
                        best_particle_index = particle_index;
                    }
                    
                    // Set personal minimum
                    if (new_value < particle_solo_best_value[particle_index]) {

                        for (int p_dimension = 0; p_dimension < input_dimension; p_dimension++) {
                            particle_solo_best_position[particle_index][p_dimension] = particle_position[particle_index][p_dimension];
                        }

                        particle_solo_best_value[particle_index] = new_value;
                    }
                }

                // Push to info
                best_answer->push_back(particle_solo_best_value[best_particle_index]);
                num_of_f_calls->push_back(optProblem->getNumCalled());

                // Timer
                auto done = std::chrono::high_resolution_clock::now();
                num_of_time_passed->push_back(std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count());
            }

            double *best_solution = new double[input_dimension];

            for (int i = 0; i < input_dimension; i++) {
                best_solution[i] = particle_position[best_particle_index][i];
            }

            return best_solution;
        }

    private:
        int num_of_particles = 0;
        int num_of_iterations = 0;
        double inertia_weight, r_1_weight, r_2_weight;
};