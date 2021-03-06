#pragma once

#include <vector>
#include <string>


namespace algorithm
{
    const double LOWER_BOUND = 0;
    const double UPPER_BOUND = 1;

/**
 * TESTED PARAMETERS:
 * N = 200, 500;
 * ACCURACY = 1E-5;
 * MAX_ITERATIONS_NUMBER = 30000;
 * MIN_RELAXATION_PARAMETER = 0.05;
 * MIN_RELAXATION_PARAMETER = 0.05;
 * RELAXATION_MULTIPLIER = 0.5;
 * INITIAL_TAU = 1;
 * INITIAL_U = 3, 100, 400;
 * INITIAL_B0 = 1;
 * INITIAL_A1 = 0, 1, 3, 6;
 * INITIAL_A2 = 0, 0.05, 1, 3, 6;
 * INITIAL_ALPHA = M_PI_4, M_PI_2;
 */
    class InitialParameters
    {
    public:
        const int N;
        const double STEP;
        const double ACCURACY;
        const int MAX_ITERATIONS_NUMBER;
        const double MIN_RELAXATION_PARAMETER;
        const double RELAXATION_MULTIPLIER;

        const double INITIAL_TAU;
        const double INITIAL_U;
        const double INITIAL_B0;
        const double INITIAL_A1;
        const double INITIAL_A2;
        const double INITIAL_ALPHA;

        InitialParameters() :
                N(0), STEP(0), ACCURACY(0), MAX_ITERATIONS_NUMBER(0), MIN_RELAXATION_PARAMETER(0),
                RELAXATION_MULTIPLIER(0), INITIAL_TAU(0), INITIAL_U(0), INITIAL_B0(0), INITIAL_A1(0), INITIAL_A2(0),
                INITIAL_ALPHA(0) {}

        InitialParameters(
                int N, double ACCURACY, int MAX_ITERATIONS_NUMBER, double MIN_RELAXATION_PARAMETER,
                double RELAXATION_MULTIPLIER, double INITIAL_TAU, double INITIAL_U, double INITIAL_B0,
                double INITIAL_A1, double INITIAL_A2, double INITIAL_ALPHA)
                :
                N(N), STEP((UPPER_BOUND - LOWER_BOUND) / N), ACCURACY(ACCURACY),
                MAX_ITERATIONS_NUMBER(MAX_ITERATIONS_NUMBER), MIN_RELAXATION_PARAMETER(MIN_RELAXATION_PARAMETER),
                RELAXATION_MULTIPLIER(RELAXATION_MULTIPLIER), INITIAL_TAU(INITIAL_TAU), INITIAL_U(INITIAL_U),
                INITIAL_B0(INITIAL_B0), INITIAL_A1(INITIAL_A1), INITIAL_A2(INITIAL_A2), INITIAL_ALPHA(INITIAL_ALPHA) {}
    };

    struct Variables
    {
        std::vector<double> s;
        std::vector<double> r;
        std::vector<double> z;
        std::vector<double> beta;
        double L;
        double TAU;
        double U;
        double B0;
        double A1;
        double A2;
        double ALPHA;
    };


    struct IterationInfo
    {
        long long index;
        Variables variables;

        std::string mainValueName;

        double getDoubleValueByName(const std::string& valueName);

        double getMainDoubleValue();
    };


    struct TargetParameter
    {
        std::string parameterName;
        double targetValue;
        double step;
        long long drawRate;
    };
}
