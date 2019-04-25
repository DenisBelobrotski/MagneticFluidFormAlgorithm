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

    const int N = 200; //500
    const double STEP = (UPPER_BOUND - LOWER_BOUND) / N;
    const double ACCURACY = 1E-5;
    const int MAX_ITERATIONS_NUMBER = 30000;
    const double MIN_RELAXATION_PARAMETER = 0.05;
    const double RELAXATION_MULTIPLIER = 0.5;

    const double INITIAL_TAU = 1; //1
    const double INITIAL_U = 3; //100, 400
    const double INITIAL_B0 = 1; //1
    const double INITIAL_A1 = 6; //0, 1, 3, 6
    const double INITIAL_A2 = 0.05; //0, 1, 3, 6
    const double INITIAL_ALPHA = M_PI_4; //M_PI_4, M_PI_2


    class InitialParameters
    {
    public:
        const int a;

        InitialParameters():a(0){}

        InitialParameters(int a):a(a){}
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
                std::vector<IterationInfo>* iterationsInfo);

        virtual ~DifferenceMethod();

        void calcResult();

        void setIsNeedResetTau(bool isNeedResetTau);

        bool getIsNeedResetTau();

        void setIterationFinishedCallback(std::function<void(long long, long long)>* iterationFinishedCallback);

        std::function<void(long long, long long)>* getIterationFinishedCallback();

        void setTargetParameters(std::vector<TargetParameter>* targetParameters);

        std::vector<TargetParameter>* getTargetParameters();

        void setInitialParameters(InitialParameters* initialParameters);

        InitialParameters* getInitialParameters();

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
                double &parameter, double target, double step, long long drawRate, std::string parameterName)
        noexcept(false);

        void increaseParameter(double &parameter, double target, double step, long long drawRate) noexcept(false);

        void decreaseParameter(double &parameter, double target, double step, long long drawRate) noexcept(false);

        void runExperiment(long long drawRate) noexcept(false);

        void pushExperimentResults(long long drawRate);

        double* getVariableParameterPointerByName(const std::string &parameterName);
    };
}
