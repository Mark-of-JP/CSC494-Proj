#include <Eigen/Dense>
#include <random>

struct normal_random_variable
{
    normal_random_variable(Eigen::MatrixXd const& covar)
        : normal_random_variable(Eigen::VectorXd::Zero(covar.rows()), covar)
    {}

    normal_random_variable(Eigen::VectorXd const& mean, Eigen::MatrixXd const& covar)
        : mean(mean)
    {
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigenSolver(covar);
        transform = eigenSolver.eigenvectors() * eigenSolver.eigenvalues().cwiseSqrt().asDiagonal();
    }

    Eigen::VectorXd mean;
    Eigen::MatrixXd transform;

    Eigen::VectorXd operator()() const
    {
        static std::mt19937 gen{ std::random_device{}() };
        static std::normal_distribution<> dist;

        return mean + transform * Eigen::VectorXd{ mean.size() }.unaryExpr([&](auto x) { return dist(gen); });
    }
};

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
                std::cout << i << " -> " << this->term_degrees[i]  << "+" << other->term_degrees[i] << std::endl;
                new_term->term_degrees[i] = this->term_degrees[i] + other->term_degrees[i];
                std::cout << i << " -> " << new_term->term_degrees[i] << std::endl;
            }

            return new_term;
        }

        PolynomialTerm* Integrate() {
            PolynomialTerm *antideriv = new PolynomialTerm(this->num_of_terms, this->coefficient);

            double new_coefficient = this->coefficient;   

            for (int i = 0; i < this->num_of_terms; i++) {
                new_coefficient = new_coefficient / (this->term_degrees[i] + 1);
                    
                antideriv->term_degrees[i] = this->term_degrees[i] + 1;
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
                // std::cout << "BOOP > " << i << "/" <<  this->term_degrees[i] << std::endl;
            }

            return to_string;
        }

    private:
        double coefficient;
        int* term_degrees;
        uint num_of_terms;
};