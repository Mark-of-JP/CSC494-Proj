#include "opt_solver.h"
#include <unsupported/Eigen/MatrixFunctions>

class CMA_ES_Solver : public OptSolver {
    public:

        CMA_ES_Solver(uint num_of_samples, double step_size, double sample_percent_taken, uint num_of_steps) 
            : OptSolver() {

            this->num_of_samples = num_of_samples;
            this->step_size = step_size;
            this->sample_percent_taken = sample_percent_taken;
            this->num_of_samples_kept = sample_percent_taken * num_of_samples;
            this->num_of_steps = num_of_steps;
        }

        double* solve(OptProblem *optProblem, uint randomSeed, std::vector<double> *best_answer, std::vector<int> *num_of_f_calls, std::vector<int> *num_of_time_passed) override {

            // Record Time
            auto started = std::chrono::high_resolution_clock::now();

            uint input_dimension = optProblem->getInputDimension();

            double *mean = optProblem->generateRandomFeasibleInputs(1, randomSeed)[0];
            Eigen::Map<Eigen::VectorXd> mean_vector(mean, input_dimension);
            Eigen::MatrixXd covariance_matrix = Eigen::MatrixXd::Identity(input_dimension, input_dimension);
            Eigen::MatrixXd samples_taken[num_of_samples];

            double f_vals[num_of_samples];
            std::vector<uint> sample_order;

            // Updating evolution path
            double p_sigma_array[input_dimension] = {0};
            Eigen::Map<Eigen::VectorXd> p_sigma(p_sigma_array, input_dimension);

            // Mu W
            double mu_w = (double) this->num_of_samples_kept;
            double c_sigma = 3.0 / (double) this->num_of_samples; 

            for (int curr_step = 0; curr_step < this->num_of_steps; curr_step++) {

                sample_order.clear();

                normal_random_variable sample { covariance_matrix };
                for (uint sample_index = 0; sample_index < num_of_samples; sample_index++) {
                    samples_taken[sample_index] = (sample() * step_size) + mean_vector;
                }

                
                for (int sample_index = 0; sample_index < num_of_samples; sample_index++) {
                    f_vals[sample_index] = optProblem->f(samples_taken[sample_index].data());

                    bool has_ordered = false;
                    for (int sample_order_index = 0; sample_order_index < sample_order.size(); sample_order_index++) {
                        
                        if (f_vals[sample_index] < f_vals[sample_order[sample_order_index]]) {
                            sample_order.insert(sample_order.begin() + sample_order_index, sample_index);
                            has_ordered = true;
                            break;
                        }
                    }

                    if (!has_ordered) {
                        sample_order.push_back(sample_index);
                    }

                }

                for (int sample_index = 0; sample_index < num_of_samples; sample_index++) {
                    // std::cout << "Order " << f_vals[sample_order[sample_index]] << std::endl;
                }

                Eigen::VectorXd aux_vector(input_dimension);
                for (int sample_index = 0; sample_index < num_of_samples_kept; sample_index++) {
                    aux_vector += (samples_taken[sample_order[sample_index]] - mean_vector);
                }
                aux_vector = aux_vector / num_of_samples_kept;


                //Moving mean
                mean_vector += (aux_vector);
                
                Eigen::MatrixXd c_inverse = covariance_matrix.inverse();
                Eigen::MatrixXd c_inverse_sqrt = c_inverse.sqrt();

                p_sigma = (1 - c_sigma) * p_sigma + sqrt(1 - (1 - c_sigma) * (1 - c_sigma)) * sqrt(mu_w) * c_inverse_sqrt * (aux_vector / step_size);

                covariance_matrix = (0.8 * covariance_matrix) + (0.2) * (aux_vector / step_size) * (aux_vector.transpose() / step_size);

                step_size *= exp(c_sigma / 1.1 * ((p_sigma.norm() / (sqrt(this->num_of_samples) * (1 - 1.0 / ((double) 4 * this->num_of_samples) + 1.0 / ((double) 21 * this->num_of_samples * this->num_of_samples)))) - 1));

                // Push to info
                best_answer->push_back(f_vals[sample_order[0]]);
                num_of_f_calls->push_back(optProblem->getNumCalled());

                // Timer
                auto done = std::chrono::high_resolution_clock::now();
                num_of_time_passed->push_back(std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count());
            }

            double *best_solution = new double[input_dimension];

            for (int i = 0; i < input_dimension; i++) {
                best_solution[i] = samples_taken[sample_order[0]].data()[i];
            }

            return best_solution;
        }

    private:
        uint num_of_samples;
        double step_size;
        double sample_percent_taken;
        uint num_of_samples_kept;
        uint num_of_steps;
};