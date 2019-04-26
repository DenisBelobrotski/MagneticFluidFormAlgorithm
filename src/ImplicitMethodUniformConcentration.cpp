#include "MagneticFluidFormAlgorithm/ImplicitMethodUniformConcentration.h"


algorithm::ImplicitMethodUniformConcentration::ImplicitMethodUniformConcentration() : ImplicitDifferenceMethod()
{
}


algorithm::ImplicitMethodUniformConcentration::ImplicitMethodUniformConcentration(
        std::vector<Variables>* experimentVariables,
        std::vector<IterationInfo>* iterationsInfo,
        InitialParameters* initialParameters)
        :
        ImplicitDifferenceMethod(experimentVariables, iterationsInfo, initialParameters)
{
}


algorithm::ImplicitMethodUniformConcentration::~ImplicitMethodUniformConcentration() = default;


#pragma MARK - Formulas

double algorithm::ImplicitMethodUniformConcentration::calcLowerGamma(double I1, double I2)
{
    double sum = variables.r[0] * cos(initialParameters->INITIAL_ALPHA) -
                 variables.r[initialParameters->N] * sin(initialParameters->INITIAL_ALPHA) -
                 variables.B0 * variables.U / (2 * M_PI * variables.L) + variables.A1 * I2;

    return 2 * sum / (variables.r[initialParameters->N] * variables.r[initialParameters->N] -
                      variables.r[0] * variables.r[0]);
}


double algorithm::ImplicitMethodUniformConcentration::calcUpperPhi(int i, double lowerGamma, double I1)
{
    double sum = -variables.A1 * calcLowerPhi(i) + lowerGamma;

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
