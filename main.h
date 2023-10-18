typedef unsigned int uint;

enum class OptimizationType {
    CONTINUOUS,
    DISCRETE
};

class OptProblem {
    public:
        OptProblem(OptimizationType optType) : optimization_type(optType) {}

        virtual double f(double *input) = 0;
        virtual uint getInputDimension() = 0;
        virtual double* getInputConstraints() = 0;

        virtual double** generateRandomFeasibleInputs(uint num_of_inputs, uint randomSeed) = 0;

    private:
        OptimizationType optimization_type;
};