#include "opt_solver.h"
#include "../utils.h"

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

        double* solve(OptProblem *optProblem, uint randomSeed) override {

            uint input_dimension = optProblem->getInputDimension();

            double mean[input_dimension] = { 0 };
            Eigen::Map<Eigen::VectorXd> mean_vector(mean, input_dimension);
            Eigen::MatrixXd covariance_matrix = Eigen::MatrixXd::Identity(input_dimension, input_dimension);
            Eigen::MatrixXd samples_taken[num_of_samples];

            double f_vals[num_of_samples];
            std::vector<uint> sample_order;

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
                    std::cout << "Order " << f_vals[sample_order[sample_index]] << std::endl;
                }


                Eigen::VectorXd aux_vector(input_dimension);
                for (int sample_index = 0; sample_index < num_of_samples_kept; sample_index++) {
                    aux_vector += samples_taken[sample_order[sample_index]];
                }
                aux_vector = aux_vector / num_of_samples_kept;

                std::cout << "Aux " << aux_vector << std::endl;


                //Moving mean
                mean_vector += (step_size * aux_vector);
                covariance_matrix = (0.8 * covariance_matrix) + 0.2 * aux_vector * aux_vector.transpose();

                std::cout << "Mean " << mean_vector << std::endl;
            }

            std::cout << samples_taken[sample_order[0]] << " -> " << f_vals[sample_order[0]] << std::endl;

            return samples_taken[sample_order[0]].data();
        }

    private:
        uint num_of_samples;
        double step_size;
        double sample_percent_taken;
        uint num_of_samples_kept;
        uint num_of_steps;
};