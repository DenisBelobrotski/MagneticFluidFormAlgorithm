#pragma once

#define _USE_MATH_DEFINES

#include <vector>
#include <math.h>
#include <string>
#include <functional>

#include <MagneticFluidFormAlgorithm/Utils.h>


namespace algorithm
{


    class DifferenceMethod
    {
    protected:
        Variables variables;
        long long iterationsCounter;
        long long experimentsCounter;
        std::function<void(long long, long long)>* iterationFinishedCallback;
        InitialParameters* initialParameters;

    private:
        std::vector<IterationInfo>* iterationsInfo;
        std::vector<algorithm::TargetParameter>* targetParameters;
        bool isLastExperiment;
        bool isNeedResetTau;
        std::string currentParameterName;

    public:
        DifferenceMethod(
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
                double& parameter, double target, double step, long long drawRate, const std::string& parameterName)
        noexcept(false);

        void increaseParameter(double& parameter, double target, double step, long long drawRate) noexcept(false);

        void decreaseParameter(double& parameter, double target, double step, long long drawRate) noexcept(false);

        void runExperiment(long long drawRate) noexcept(false);

        void pushExperimentResults();

        double* getVariableParameterPointerByName(const std::string& parameterName);
    };
}
