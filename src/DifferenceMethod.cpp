#include "MagneticFluidFormAlgorithm/DifferenceMethod.h"
#include "Formulas.h"

#include <iostream>
#include <string>

#include <MagneticFluidFormAlgorithm/Exceptions.h>


#pragma MARK - Object lifecycle

algorithm::DifferenceMethod::DifferenceMethod() = default;


algorithm::DifferenceMethod::DifferenceMethod(
        std::vector<Variables>* experimentVariables,
        std::vector<IterationInfo>* iterationsInfo,
        InitialParameters* initialParameters)
{
    if (experimentVariables == nullptr)
    {
        throw std::runtime_error("DifferenceMethod: experimentVariables is null");
    }
    if (iterationsInfo == nullptr)
    {
        throw std::runtime_error("DifferenceMethod: iterationsInfo is null");
    }
    if (initialParameters == nullptr)
    {
        throw std::runtime_error("DifferenceMethod: initialParameters is null");
    }

    this->experimentVariables = experimentVariables;
    this->iterationsInfo = iterationsInfo;
    this->initialParameters = initialParameters;

    variables.s = std::vector<double>(initialParameters->N + 1);
    variables.r = std::vector<double>(initialParameters->N + 1);
    variables.z = std::vector<double>(initialParameters->N + 1);
    variables.beta = std::vector<double>(initialParameters->N + 1);

    isNeedResetTau = false;
}


algorithm::DifferenceMethod::~DifferenceMethod() = default;


#pragma MARK - Private mathods

void algorithm::DifferenceMethod::fillVariables()
{
    for (auto i = 0; i < initialParameters->N + 1; i++)
    {
        variables.s[i] = LOWER_BOUND + i * initialParameters->STEP;

        variables.r[i] = 1 + variables.s[i];

        variables.z[initialParameters->N - i] = variables.s[i];

        variables.beta[i] = -M_PI_4;
    }

    variables.L = 0;
    variables.TAU = initialParameters->INITIAL_TAU;
    variables.U = initialParameters->INITIAL_U;
    variables.B0 = initialParameters->INITIAL_B0;
    variables.A1 = initialParameters->INITIAL_A1;
    variables.A2 = initialParameters->INITIAL_A2;
    variables.ALPHA = initialParameters->INITIAL_ALPHA;
}


void algorithm::DifferenceMethod::resetFields()
{
    iterationsCounter = 0;
    experimentsCounter = 0;
    isLastExperiment = false;
    fillVariables();
}


void algorithm::DifferenceMethod::changeParameter(
        double &parameter, double target, double step, long long drawRate, std::string parameterName) noexcept(false)
{
    if (isNeedResetTau)
    {
        variables.TAU = initialParameters->INITIAL_TAU;
    }

    bool isCalculated = false;
    const Variables startVariables = variables;

    if (drawRate != -2)
    {
        std::cout << "Start changing parameter " << parameterName << std::endl;
        std::cout << "Current value: " << parameter << ", target value: " << target << std::endl;
        std::cout << "Step: " << step << std::endl;
        std::cout << "////////////" << std::endl;
    }

    while (variables.TAU >= initialParameters->MIN_RELAXATION_PARAMETER && !isCalculated)
    {
        try
        {
            isLastExperiment = false;
            if (target > parameter)
            {
                increaseParameter(parameter, target, step, drawRate);
            }
            else if (target < parameter)
            {
                decreaseParameter(parameter, target, step, drawRate);
            }
            isCalculated = true;

        }
        catch (InvalidResultException &e)
        {
            std::cerr << "Invalid result:" << std::endl;
            std::cout << "relaxation parameter: " << std::endl;
            std::cout << "current: " << variables.TAU << ", ";
            variables.TAU *= initialParameters->RELAXATION_MULTIPLIER;
            std::cout << "new: " << variables.TAU << std::endl;
            std::cout << "-----" << std::endl;
        }
        catch (IterationsLimitException &e)
        {
            std::cerr << "Too many iterations:" << std::endl;
            std::cout << "relaxation parameter: " << std::endl;
            std::cout << "current: " << variables.TAU << ", ";
            variables.TAU *= initialParameters->RELAXATION_MULTIPLIER;
            std::cout << "new: " << variables.TAU << std::endl;
            std::cout << "-----" << std::endl;
        }

        if (!isCalculated)
        {
            double newTau = variables.TAU;
            variables = startVariables;
            variables.TAU = newTau;
        }
    }
    if (!isCalculated)
    {
        throw ParameterNotReachTargetValue(parameterName, target);
    }

    if (drawRate != -2)
    {
        std::cout << "Finish changing parameter " << parameterName << " to target value " << target << std::endl;
        std::cout << "////////////" << std::endl << std::endl;
    }
}

void algorithm::DifferenceMethod::increaseParameter(double &parameter, double target, double step, long long drawRate)
{
    while (parameter <= target)
    {
        if (parameter + step >= target)
        {
            isLastExperiment = true;
        }

        runExperiment(drawRate);

        if (isLastExperiment)
        {
            break;
        }

        parameter += step;
    }
}

void algorithm::DifferenceMethod::decreaseParameter(double &parameter, double target, double step, long long drawRate)
{
    while (parameter >= target)
    {
        if (parameter - step <= target)
        {
            isLastExperiment = true;
        }

        runExperiment(drawRate);

        if (isLastExperiment)
        {
            break;
        }

        parameter -= step;
    }
}


void algorithm::DifferenceMethod::runExperiment(long long drawRate)
{
    runIterationProcess();

    pushExperimentResults(drawRate);

    experimentsCounter++;
}


void algorithm::DifferenceMethod::pushExperimentResults(long long drawRate)
{
    IterationInfo currentIterationInfo{};
    Variables resultVariables = variables;

    convertLengthToRadiusDimensionedVariables(resultVariables);

    experimentVariables->push_back(resultVariables);

    if (
            (drawRate > 0 && experimentsCounter % drawRate == 0)
            || (isLastExperiment && drawRate != -2)
            )
    {
        currentIterationInfo.index = experimentsCounter;
        currentIterationInfo.tau = variables.TAU;
        currentIterationInfo.u = variables.U;
        currentIterationInfo.b0 = variables.B0;
        currentIterationInfo.a1 = variables.A1;
        currentIterationInfo.a2 = variables.A2;
        currentIterationInfo.alpha = variables.ALPHA;

        iterationsInfo->push_back(currentIterationInfo);
    }
}


double* algorithm::DifferenceMethod::getVariableParameterPointerByName(const std::string &parameterName)
{
    double* resultPtr = nullptr;

    if (parameterName == "U")
    {
        resultPtr = &(variables.U);
    }
    if (parameterName == "B0")
    {
        resultPtr = &(variables.B0);
    }
    if (parameterName == "A1")
    {
        resultPtr = &(variables.A1);
    }
    if (parameterName == "A2")
    {
        resultPtr = &(variables.A2);
    }
    if (parameterName == "ALPHA")
    {
        resultPtr = &(variables.ALPHA);
    }

    return resultPtr;
}


#pragma MARK - Public methods

void algorithm::DifferenceMethod::calcResult()
{
    resetFields();

    iterationsCounter = 0;
    experimentsCounter = 0;

    try
    {
        if (targetParameters != nullptr)
        {
            for (auto &targetParameter : *targetParameters)
            {
                double* variableParameterPtr = getVariableParameterPointerByName(targetParameter.parameterName);
                if (variableParameterPtr != nullptr)
                {
                    changeParameter(
                            *variableParameterPtr, targetParameter.targetValue, targetParameter.step,
                            targetParameter.drawRate, targetParameter.parameterName);
                }
            }
        }
    }
    catch (ParameterNotReachTargetValue &e)
    {
        std::cerr << "Parameter " << e.getParameterName() << " didn't reach target value "
                  << e.getTargetValue() << std::endl;
    }

    std::cout << "iterations count: " << iterationsCounter << std::endl;
    std::cout << "experiments count: " << experimentsCounter << std::endl;
    std::cout << "************************" << std::endl;
}


void algorithm::DifferenceMethod::setIsNeedResetTau(bool isNeedResetTau)
{
    this->isNeedResetTau = isNeedResetTau;
}


bool algorithm::DifferenceMethod::getIsNeedResetTau()
{
    return isNeedResetTau;
}


void algorithm::DifferenceMethod::setIterationFinishedCallback(
        std::function<void(long long, long long)>* iterationFinishedCallback)
{
    this->iterationFinishedCallback = iterationFinishedCallback;
}


std::function<void(long long, long long)>* algorithm::DifferenceMethod::getIterationFinishedCallback()
{
    return iterationFinishedCallback;
}


void algorithm::DifferenceMethod::setTargetParameters(std::vector<TargetParameter>* targetParameters)
{
    this->targetParameters = targetParameters;
}


std::vector<algorithm::TargetParameter>* algorithm::DifferenceMethod::getTargetParameters()
{
    return targetParameters;
}
