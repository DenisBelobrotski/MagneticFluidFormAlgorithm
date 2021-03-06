#include "MagneticFluidFormAlgorithm/ImplicitDifferenceMethod.h"
#include "RightSweep.h"
#include "Formulas.h"

#include <MagneticFluidFormAlgorithm/MathUtils.h>
#include <MagneticFluidFormAlgorithm/Exceptions.h>


algorithm::ImplicitDifferenceMethod::ImplicitDifferenceMethod() : DifferenceMethod()
{
}


algorithm::ImplicitDifferenceMethod::ImplicitDifferenceMethod(
        std::vector<IterationInfo>* iterationsInfo,
        InitialParameters* initialParameters)
        :
        DifferenceMethod(iterationsInfo, initialParameters)
{
}


algorithm::ImplicitDifferenceMethod::~ImplicitDifferenceMethod() = default;


#pragma MARK - Algorithm

void algorithm::ImplicitDifferenceMethod::fillMatrices(
        std::vector<std::vector<double>>& radiusMatrix, std::vector<double>& radiusFreeMembers,
        std::vector<std::vector<double>>& heightMatrix, std::vector<double>& heightFreeMembers)
{
    double I0 = calcIntegral0();
    variables.L = calcL(I0, variables.U);
    double I1 = calcIntegral1();
    double I2 = calcIntegral2();

    double lowerGamma = calcLowerGamma(I1, I2);

    double upperPhi0 = calcUpperPhi(0, lowerGamma, I1);
    double upperPhiN = calcUpperPhi((int) (initialParameters->N), lowerGamma, I1);

    heightMatrix[0][0] = 1;
    heightMatrix[0][1] = -1;
    heightFreeMembers[0] = -0.5 * initialParameters->STEP * initialParameters->STEP * upperPhi0 *
                           sin(initialParameters->INITIAL_ALPHA) +
                           initialParameters->STEP * cos(initialParameters->INITIAL_ALPHA);

    radiusMatrix[0][0] = 1;
    radiusMatrix[0][1] = 0;
    radiusFreeMembers[0] = 1 / variables.L;

    for (auto i = 1; i < initialParameters->N; i++)
    {
        double upperFi = calcUpperPhi(i, lowerGamma, I1);

        heightMatrix[i][i - 1] = 1;
        heightMatrix[i][i] = -2;
        heightMatrix[i][i + 1] = 1;
        heightFreeMembers[i] = 0.5 * initialParameters->STEP * (variables.r[i + 1] - variables.r[i - 1]) * upperFi;

        radiusMatrix[i][i - 1] = 1;
        radiusMatrix[i][i] = -2;
        radiusMatrix[i][i + 1] = 1;
        radiusFreeMembers[i] = -0.5 * initialParameters->STEP * (variables.z[i + 1] - variables.z[i - 1]) * upperFi;
    }

    heightMatrix[initialParameters->N][initialParameters->N - 1] = 0;
    heightMatrix[initialParameters->N][initialParameters->N] = 1;
    heightFreeMembers[initialParameters->N] = 0;

    radiusMatrix[initialParameters->N][initialParameters->N - 1] = -1;
    radiusMatrix[initialParameters->N][initialParameters->N] = 1;
    radiusFreeMembers[initialParameters->N] = -0.5 * initialParameters->STEP * initialParameters->STEP * upperPhiN *
                                              sin(initialParameters->INITIAL_ALPHA) +
                                              initialParameters->STEP * cos(initialParameters->INITIAL_ALPHA);
}


void algorithm::ImplicitDifferenceMethod::calcIteration()
{
    std::vector<double> radiusFreeMembers(initialParameters->N + 1);
    std::vector<double> heightFreeMembers(initialParameters->N + 1);
    std::vector<std::vector<double>> radiusMatrix(initialParameters->N + 1);
    std::vector<std::vector<double>> heightMatrix(initialParameters->N + 1);
    for (auto i = 0; i < initialParameters->N + 1; i++)
    {
        radiusMatrix[i].resize(initialParameters->N + 1);
        heightMatrix[i].resize(initialParameters->N + 1);
    }

    fillMatrices(radiusMatrix, radiusFreeMembers, heightMatrix, heightFreeMembers);

    calcRightSweep(radiusMatrix, radiusFreeMembers, variables.r);
    calcRightSweep(heightMatrix, heightFreeMembers, variables.z);
}


void algorithm::ImplicitDifferenceMethod::runIterationProcess() noexcept(false)
{
    long long startIterationsCounter = iterationsCounter;
    double residual;

    std::vector<double> prevR;
    std::vector<double> prevZ;

    do
    {
        prevR = variables.r;
        prevZ = variables.z;

        calcIteration();

        residual = std::max(calcResidual(prevR, variables.r),
                            calcResidual(prevZ, variables.z));

        for (auto i = 0; i < initialParameters->N + 1; i++)
        {
            variables.r[i] = (1 - variables.TAU) * prevR[i] + variables.TAU * variables.r[i];
            variables.z[i] = (1 - variables.TAU) * prevZ[i] + variables.TAU * variables.z[i];
        }

        iterationsCounter++;

        if (iterationsCounter - startIterationsCounter > initialParameters->MAX_ITERATIONS_NUMBER)
        {
            throw IterationsLimitException();
        }
        else
        {
            if (iterationFinishedCallback != nullptr)
            {
                (*iterationFinishedCallback)(
                        iterationsCounter - startIterationsCounter, initialParameters->MAX_ITERATIONS_NUMBER);
            }
        }
    } while (residual > initialParameters->ACCURACY);

    if (!isValid(variables.r) || !isValid(variables.z))
    {
        throw InvalidResultException();
    }
}


#pragma MARK - Formulas

double algorithm::ImplicitDifferenceMethod::calcLowerPhi(int i)
{
    if (i == 0)
    {
        return sinh(variables.A2) / variables.A2;
    }

    double tmp = variables.L * variables.r[i] / variables.A2;
    return tmp * sinh(1 / tmp);
}


double algorithm::ImplicitDifferenceMethod::calcLowerGamma(double I1, double I2)
{
    double sum = variables.r[0] * cos(initialParameters->INITIAL_ALPHA) -
                 variables.r[initialParameters->N] * sin(initialParameters->INITIAL_ALPHA) -
                 variables.B0 * variables.U / (2 * M_PI * variables.L) +
                 variables.A1 * variables.U * I2 / (I1 * variables.L * variables.L);

    return 2 * sum / (variables.r[initialParameters->N] * variables.r[initialParameters->N] -
                      variables.r[0] * variables.r[0]);
}


double algorithm::ImplicitDifferenceMethod::calcUpperPhi(int i, double lowerGamma, double I1)
{
    double sum = -variables.A1 * variables.U * calcLowerPhi(i) / (I1 * variables.L * variables.L) + lowerGamma;

    if (i == 0)
    {
        return variables.B0 * variables.L * variables.L * variables.z[i] + sum +
               variables.L * cos(initialParameters->INITIAL_ALPHA);
    }

    if (i == initialParameters->N)
    {
        return sum + sin(initialParameters->INITIAL_ALPHA) / variables.r[i];
    }

    return variables.B0 * variables.L * variables.L * variables.z[i] + sum -
           ((variables.z[i + 1] - variables.z[i - 1]) / (2 * initialParameters->STEP * variables.r[i]));
}


#pragma MARK - Integrals

double algorithm::ImplicitDifferenceMethod::calcIntegral0()
{
    double sum = 0;

    for (auto i = 1; i < initialParameters->N + 1; i++)
    {
        double height = (variables.z[i] + variables.z[i - 1]) / 2;
        double radiusQuadDiff = variables.r[i] * variables.r[i] -
                                variables.r[i - 1] * variables.r[i - 1];

        sum += height * radiusQuadDiff;
    }

    return M_PI * sum;
}

double algorithm::ImplicitDifferenceMethod::calcIntegral1()
{
    double sum = 0;

    for (auto i = 1; i < initialParameters->N + 1; i++)
    {
        double height = (variables.z[i] + variables.z[i - 1]) / 2;
        double radiusQuadDiff = variables.r[i] * variables.r[i] -
                                variables.r[i - 1] * variables.r[i - 1];
        double lowerPhi = calcLowerPhi(i);

        sum += height * radiusQuadDiff * lowerPhi;
    }

    return M_PI * sum;
}

double algorithm::ImplicitDifferenceMethod::calcIntegral2()
{
    double sum = 0;

    for (auto i = 1; i < initialParameters->N + 1; i++)
    {
        double radiusQuadDiff = variables.r[i] * variables.r[i] -
                                variables.r[i - 1] * variables.r[i - 1];
        double lowerPhi = calcLowerPhi(i);

        sum += radiusQuadDiff * lowerPhi;
    }

    return sum / 2;
}
