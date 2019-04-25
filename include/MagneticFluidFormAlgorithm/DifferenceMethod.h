#pragma once

#define _USE_MATH_DEFINES

#include <vector>
#include <math.h>
#include <string>
#include <functional>


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
        double tau;
        double u;
        double b0;
        double a1;
        double a2;
        double alpha;
    };


    struct TargetParameter
    {
        std::string parameterName;
        double targetValue;
        double step;
        long long drawRate;
    };


    class DifferenceMethod
    {
    protected:
        Variables variables;
        long long iterationsCounter;
        long long experimentsCounter;
        std::function<void(long long, long long)>* iterationFinishedCallback;
        InitialParameters* initialParameters;

    private:
        std::vector<Variables>* experimentVariables;
        std::vector<IterationInfo>* iterationsInfo;
        std::vector<algorithm::TargetParameter>* targetParameters;
        bool isLastExperiment;
        bool isNeedResetTau;

    public:
        DifferenceMethod(
                std::vector<Variables>* experimentVariables,
                std::vector<IterationInfo>* iterationsInfo,
                InitialParameters* initialParameters);

        virtual ~DifferenceMethod();

        void calcResult();

        void setIsNeedResetTau(bool isNeedResetTau);

        bool getIsNeedResetTau();

        void setIterationFinishedCallback(std::function<void(long long, long long)>* iterationFinishedCallback);

        std::function<void(long long, long long)>* getIterationFinishedCallback();

        void setTargetParameters(std::vector<TargetParameter>* targetParameters);

        std::vector<TargetParameter>* getTargetParameters();

    protected:
        DifferenceMethod();

        virtual void runIterationProcess() = 0;

    private:

        void fillVariables();

        void resetFields();

        /**
         * @param drawRate:
         * -2 doesn't push any experiments
         * -1 pushes only last experiment
         */
        void changeParameter(
                double& parameter, double target, double step, long long drawRate, std::string parameterName)
        noexcept(false);

        void increaseParameter(double& parameter, double target, double step, long long drawRate) noexcept(false);

        void decreaseParameter(double& parameter, double target, double step, long long drawRate) noexcept(false);

        void runExperiment(long long drawRate) noexcept(false);

        void pushExperimentResults(long long drawRate);

        double* getVariableParameterPointerByName(const std::string& parameterName);
    };
}
