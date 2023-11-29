#include "opt_problem.h"

// Optimal solution is (0, -2.67298)
class UncapacitatedRandomEquation : public OptProblem {
    public:
        UncapacitatedRandomEquation() 
        : OptProblem(OptimizationType::CONTINUOUS) {}

        double f_implement(double *input) override {
            double x = input[0];
            double y = input[1];

            return (x*x - 5) + (y * y * y * y + 4 * y * y * y + y * y - 4 * y) + (x * x * y * y);
        };
        int getInputDimension() override { return 2; };
        double* getInputConstraints() override { 
            // Do not trust this. I should malloc
            double constraints[2] = { 9999.0, 9999.0 };

            return constraints; 
        };


        double* getBestInput() override {
            double best_input[2] = { 0, -2.67298};

            return best_input;
        }


        double** generateRandomFeasibleInputs(uint num_of_inputs, uint randomSeed) override { 
            double **feasible_inputs = new double*[num_of_inputs]; 
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

        void writeToFile(std::string pathname) {
            std::vector<int> problemInfo;

            // Insert number of variables
            problemInfo.push_back(this->getInputDimension());

            // Insert constraints
            double *constraints = this->getInputConstraints();
            for (int i = 0; i < this->getInputDimension(); i++) {
                problemInfo.push_back(constraints[i]);
            }


            // Writing too file
            std::ofstream file(
                pathname, std::ios::out | std::ios::binary | std::ios::trunc
            );
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open file for writing.");
            }

            for (const auto& problemValue : problemInfo) {
                file.write(reinterpret_cast<const char*>(&problemValue), sizeof(int));
            }

            file.close();
        };
};



