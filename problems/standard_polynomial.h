#include "opt_problem.h"
#include <math.h>

struct PolynomialTerm {

    public:
        PolynomialTerm(uint num_of_terms, double coefficient = 1) {
            this->coefficient = coefficient;
            this->num_of_terms = num_of_terms;
            this->term_degrees = new int[num_of_terms];

            for (int i = 0; i < num_of_terms; i++) {
                this->term_degrees[i] = 0;
            }
        }

        ~PolynomialTerm() {
            delete [] this->term_degrees;
        }

        void SetTermDegree(uint term_index, int degree) {
            this->term_degrees[term_index] = degree;
        }

        PolynomialTerm *Multiply(PolynomialTerm *other) {
            PolynomialTerm *new_term = new PolynomialTerm(this->num_of_terms, this->coefficient * other->coefficient);

            for (int i = 0; i < this->num_of_terms; i++) {
                new_term->term_degrees[i] = this->term_degrees[i] + other->term_degrees[i];
            }

            return new_term;
        }

        PolynomialTerm* Integrate() {
            PolynomialTerm *antideriv = new PolynomialTerm(this->num_of_terms, this->coefficient);

            double new_coefficient = this->coefficient;   

            bool has_term = false;
            for (int i = 0; i < this->num_of_terms; i++) {
                if (this->term_degrees[i] > 0) {
                    has_term = true;
                }
            }

            if (!has_term) {
                antideriv->coefficient = 0;
                return antideriv;
            }

            for (int i = 0; i < this->num_of_terms; i++) {
                if (this->term_degrees[i] > 0) {
                    new_coefficient = new_coefficient * this->term_degrees[i];
                    
                    antideriv->term_degrees[i] = this->term_degrees[i] - 1;
                }
            }

            antideriv->coefficient = new_coefficient;

            return antideriv;
        }

        double Evaluate(double *terms) {
            double value = this->coefficient;

            for (int i = 0; i < this->num_of_terms; i++) {
                value = value * pow(terms[i], this->term_degrees[i]);
            }

            return value;
        }

        std::string toString() {
            std::string to_string = std::to_string(this->coefficient);

            for (int i = 0; i < this->num_of_terms; i++) {
                if (this->term_degrees[i] > 0) {
                    to_string += "*(" + std::to_string(i) + ")^" + std::to_string(this->term_degrees[i]);
                }
            }

            return to_string;
        }

    private:
        double coefficient;
        int* term_degrees;
        uint num_of_terms;
};

// Optimal solution is (0, -2.67298)
class StandardPolynomial : public OptProblem {
    public:
        StandardPolynomial(uint num_of_terms, uint num_of_zeroes, double randomSeed) 
        : OptProblem(OptimizationType::CONTINUOUS) {

            deriv_term = new uint[num_of_zeroes];
            term_zero = new double[num_of_zeroes];

            std::vector<PolynomialTerm*> poly_terms;

            srand(randomSeed); 

            for (int i = 0; i < num_of_zeroes; i++) {
                deriv_term[i] = rand() % num_of_terms;
                // term_zero[i] = ((double) rand() / RAND_MAX) + ((rand() % 20) - 10);
                term_zero[i] = ((rand() % 20) - 10);

                PolynomialTerm *term = new PolynomialTerm(num_of_terms, 1);
                term->SetTermDegree(deriv_term[i], 1);
                poly_terms.push_back(term);

                PolynomialTerm *zero_term = new PolynomialTerm(num_of_terms, term_zero[i]);
                poly_terms.push_back(zero_term);
            }
            std::cout << std::endl;

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
                this->expanded_terms.insert(std::end(this->expanded_terms), std::begin(new_terms), std::end(new_terms));
            }

            
            //Get the antiderivative
            std::vector<PolynomialTerm*> antideriv;

            for (int i = 0; i < this->expanded_terms.size(); i++) {
                antideriv.push_back(this->expanded_terms[i]->Integrate());
            }

            this->expanded_terms.clear();
            this->expanded_terms.insert(std::end(this->expanded_terms), std::begin(antideriv), std::end(antideriv));
        }

        ~StandardPolynomial() {
            this->expanded_terms.clear();
        }

        double f_implement(double *input) override {
            double value = 0;

            for (int i = 0; i < this->num_of_terms; i++) {
                value += expanded_terms[i]->Evaluate(input);
            }

            return value;
        };

        int getInputDimension() override { return this->num_of_terms; };
        double* getInputConstraints() override { 
            // Do not trust this. I should malloc
            double constraints[2] = { 9999.0, 9999.0 };

            return constraints; 
        };

        double* getBestInput() override {
            
            std::vector<double*> feasible_best_inputs;

            // Check 0's
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

    private:
        uint num_of_terms;
        std::vector<PolynomialTerm*> expanded_terms;
        uint *deriv_term;
        double *term_zero;

};