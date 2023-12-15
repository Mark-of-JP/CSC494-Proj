#include "opt_problem.h"

// Optimal solution is (0, -2.67298)
class StandardPolynomial : public OptProblem {
    public:
        StandardPolynomial(uint num_of_terms, uint num_of_zeroes, double randomSeed) 
        : OptProblem(OptimizationType::CONTINUOUS) {

            this->num_of_terms = num_of_terms;
            this->num_of_zeroes = num_of_zeroes;

            // Initializing Constraints at 0. U can set it with different functions
            this->constraints = new double[num_of_terms * 2];

            deriv_term = new uint[num_of_zeroes];
            term_zero = new double[num_of_zeroes];

            std::vector<PolynomialTerm*> poly_terms;

            srand(randomSeed); 

            for (int i = 0; i < num_of_zeroes; i++) {
                deriv_term[i] = rand() % num_of_terms;
                term_zero[i] = ((double) rand() / RAND_MAX) + ((rand() % 6) - 3);
                // term_zero[i] = ((rand() % 100) - 50);

                PolynomialTerm *term = new PolynomialTerm(num_of_terms, 1);
                term->SetTermDegree(deriv_term[i], 1);
                poly_terms.push_back(term);

                PolynomialTerm *zero_term = new PolynomialTerm(num_of_terms, term_zero[i]);
                poly_terms.push_back(zero_term);
            }
            // std::cout << std::endl;

            // Added the extra terms
            for (int i = 0; i < this->num_of_terms; i++) {
                PolynomialTerm *term = new PolynomialTerm(num_of_terms, 1);
                term->SetTermDegree(deriv_term[i], 1);
                expanded_terms.push_back(term);
            }

            for (int initial_terms = 0; initial_terms < num_of_zeroes; initial_terms++) {
                
                // Mutliply with polyterms
                PolynomialTerm *curr_var_term = poly_terms[initial_terms * 2];
                PolynomialTerm *curr_zero_term = poly_terms[(initial_terms * 2) + 1];

                std::vector<PolynomialTerm*> new_terms;

                for (int curr_poly_term_index = ((initial_terms + 1) * 2); curr_poly_term_index < num_of_zeroes * 2; curr_poly_term_index++) {
                    PolynomialTerm *new_var_term = curr_var_term->Multiply(poly_terms[curr_poly_term_index]);
                    PolynomialTerm *new_zero_term = curr_zero_term->Multiply(poly_terms[curr_poly_term_index]);

                    new_terms.push_back(new_var_term);
                    new_terms.push_back(new_zero_term);
                }

                // Multiply with expanded terms
                for (int curr_expanded_term_index = 0; curr_expanded_term_index < this->expanded_terms.size(); curr_expanded_term_index++) {
                    PolynomialTerm *new_var_term = curr_var_term->Multiply(this->expanded_terms[curr_expanded_term_index]);
                    PolynomialTerm *new_zero_term = curr_zero_term->Multiply(this->expanded_terms[curr_expanded_term_index]);

                    new_terms.push_back(new_var_term);
                    new_terms.push_back(new_zero_term);
                }

                this->expanded_terms.clear();

                // Collect like terms
                for (int new_terms_1 = 0; new_terms_1 < new_terms.size(); new_terms_1++) {

                    PolynomialTerm *collected_term = new_terms[new_terms_1];

                    if (collected_term == nullptr) {
                        continue;
                    }

                    for (int new_terms_2 = new_terms_1 + 1; new_terms_2 < new_terms.size(); new_terms_2++) {

                        PolynomialTerm *addition_term = new_terms[new_terms_2];

                        if (addition_term == nullptr) {
                            continue;
                        }

                        bool is_collected = collected_term->Add(addition_term, collected_term);

                        if (is_collected) {
                            delete addition_term;

                            new_terms[new_terms_2] = nullptr;
                        }
                    }

                    this->expanded_terms.push_back(collected_term);
                }

                // this->expanded_terms.insert(std::end(this->expanded_terms), std::begin(new_terms), std::end(new_terms));
            }

            // std::cout << "\nbefore derivative: " << std::endl;
            // for (int i = 0; i < this->expanded_terms.size(); i++) {
            //     std::cout << " + " << this->expanded_terms[i]->toString();
            // }
            // std::cout << std::endl;

            
            //Get the antiderivative
            std::vector<PolynomialTerm*> antideriv;

            for (int i = 0; i < this->expanded_terms.size(); i++) {
                antideriv.push_back(this->expanded_terms[i]->Integrate());
            }

            this->expanded_terms.clear();
            this->expanded_terms.insert(std::end(this->expanded_terms), std::begin(antideriv), std::end(antideriv));

            // Setting up penalty
            this->worst_point = this->f_implement(this->getBestInput(false));
        }

        void SetConstraints(double *new_constraints) {
            for (int i = 0; i < this->num_of_terms * 2; i++) {
                this->constraints[i] = new_constraints[i];
            }

            // Setting up penalty
            this->worst_point = this->f_implement(this->getBestInput(false));
        }
        void SetRandomConstraints(std::vector<int> term_index, std::vector<double> lower_bound, std::vector<double> upper_bound, double random_seed) {
            double constraint_limits[this->num_of_terms * 2];

            // Default constraint values
            for (int i = 0; i < this->num_of_terms; i++) {
                constraint_limits[i * 2] = -10;
                constraint_limits[(i * 2) + 1] = 10;
            }

            // Applying Set constraint bounds
            for (int i = 0; i < term_index.size(); i++) {
                constraint_limits[term_index[i] * 2] = lower_bound[i];
                constraint_limits[(term_index[i] * 2) + 1] = upper_bound[i];
            }

            // Generating Random constraints
            srand(random_seed);
            for (int i = 0; i < this->num_of_terms; i++) {
                // Develop lower bound
                uint length = constraint_limits[(i * 2) + 1] - constraint_limits[i * 2] - 2;

                double lower_constraint = ((double) rand() / RAND_MAX) + (rand() % length) + constraint_limits[i * 2];

                // Develop upper Bound
                length = constraint_limits[(i * 2) + 1] - lower_constraint - 1;

                double upper_constraint = ((double) rand() / RAND_MAX) + (rand() % length) + lower_constraint + 2;

                // Setting bounds
                this->constraints[i * 2] = lower_constraint;
                this->constraints[(i * 2) + 1] = upper_constraint;
            }

            // Setting up penalty
            this->worst_point = this->f_implement(this->getBestInput(false));
        }

        ~StandardPolynomial() {
            this->expanded_terms.clear();
        }

        void PrintEquation() {
            // Print Constraints
            std::cout << "Constraints: " << std::endl;
            for (int i = 0; i < this->num_of_terms; i++) {
                std::cout << i << " -> [" << this->constraints[i * 2]  << ", " << this->constraints[(i * 2) + 1] << "]" << std::endl;
            }

            std::cout << "\nTerms: " << std::endl;
            for (int i = 0; i < this->num_of_zeroes; i++) {
                std::cout << i << " -> (" << this->deriv_term[i]  << " + (" << this->term_zero[i] << ")" << std::endl;
            }

            std::cout << "\nEquation: " << std::endl;
            for (int i = 0; i < this->expanded_terms.size(); i++) {
                std::cout << " + " << this->expanded_terms[i]->toString();
            }
            std::cout << std::endl;
        }

        double f_implement(double *input) override {

            // If not within bounds then apply penalty
            if (!this->isWithinBounds(input)) {
                double penalty_value = 2 * this->worst_point;

                for (int i = 0; i < this->num_of_terms; i++) {
                    if (input[i] < this->constraints[i * 2]) {
                        penalty_value += this->constraints[i * 2] - input[i];
                    }
                    if (input[i] > this->constraints[(i * 2) + 1]) {
                        penalty_value += input[i] - this->constraints[(i * 2) + 1];
                    }
                }

                return penalty_value;
            }

            // If within bounds then okay
            double value = 0;

            for (int i = 0; i < this->num_of_terms; i++) {
                value += expanded_terms[i]->Evaluate(input);
            }

            

            return value;
        };

        bool isWithinBounds(double *input) override {

            for (int i = 0; i < this->num_of_terms; i++) {

                if (input[i] < this->constraints[i * 2] || this->constraints[(i * 2) + 1] < input[i]) {
                    return false;
                }

            }

            return true;
        }

        int getInputDimension() override { return this->num_of_terms; };
        double* getInputConstraints() override { 

            return this->constraints; 
        };

        double* getBestInput(bool getLeast = true) override {
            

            std::vector<double> *feasible_best_inputs = new std::vector<double>[this->num_of_terms];

            // Check constraints
            for (int i = 0; i < this->num_of_terms; i++) {
                feasible_best_inputs[i].push_back(this->constraints[(i * 2)]);
                feasible_best_inputs[i].push_back(this->constraints[(i * 2) + 1]);
                feasible_best_inputs[i].push_back(0);
            }
            
            // Check zeroes
            for (int i = 0; i < this->num_of_zeroes; i++) {
                feasible_best_inputs[deriv_term[i]].push_back(-1 * term_zero[i]);
            }

            // Looping through all possibities
            int term_index_tracker[this->num_of_terms] = {0};

            int num_of_feasible_best_input = 1;
            for (int i = 0; i < this->num_of_terms; i++) {
                num_of_feasible_best_input = num_of_feasible_best_input * feasible_best_inputs[i].size();
            }

            double best_val = 0;
            double *best_input = new double[this->num_of_terms];

            for (int i = 0; i < num_of_feasible_best_input; i++) {
                double curr_input[this->num_of_terms];

                for (int input_dimension_index = 0; input_dimension_index < this->num_of_terms; input_dimension_index++) {
                    curr_input[input_dimension_index] = feasible_best_inputs[input_dimension_index][term_index_tracker[input_dimension_index]];
                }

                double curr_val = this->f_implement(curr_input);

                if (i == 0 || (curr_val < best_val && getLeast) || (curr_val > best_val && !getLeast)) {
                    best_val = curr_val;

                    for (int input_dimension_index = 0; input_dimension_index < this->num_of_terms; input_dimension_index++) {
                        best_input[input_dimension_index] = curr_input[input_dimension_index];
                    }
                }


                // Getting next feasible input
                for (int input_dimension_index = 0; input_dimension_index < this->num_of_terms; input_dimension_index++) {
                    term_index_tracker[input_dimension_index] = term_index_tracker[input_dimension_index] + 1;

                    if (term_index_tracker[input_dimension_index] >= feasible_best_inputs[input_dimension_index].size()) {
                        term_index_tracker[input_dimension_index] = 0;
                    } else {
                        break;
                    }
                }
            }


            return best_input;
        }

        double** generateRandomFeasibleInputs(uint num_of_inputs, uint randomSeed) override { 
            double **feasible_inputs = new double*[num_of_inputs]; 
            uint input_dimension = getInputDimension();

            srand(randomSeed); 

            for (uint i = 0; i < num_of_inputs; i++) {
                feasible_inputs[i] = new double[input_dimension];

                for (uint j = 0; j < input_dimension; j++) {
                    // std::cout << this->constraints[(j * 2) + 1] - this->constraints[j * 2] << std::endl;
                    int dimension_length = (int) this->constraints[(j * 2) + 1] - this->constraints[j * 2];

                    feasible_inputs[i][j] = (rand() % dimension_length) + this->constraints[j * 2];
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

    private:
        uint num_of_terms;
        uint num_of_zeroes;

        std::vector<PolynomialTerm*> expanded_terms;
        uint *deriv_term;
        double *term_zero;
        double *constraints;
        double worst_point; // Used for penalty term

};