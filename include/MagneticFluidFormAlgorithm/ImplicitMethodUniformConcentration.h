#pragma once

#include "ImplicitDifferenceMethod.h"


namespace algorithm
{
    class ImplicitMethodUniformConcentration : public ImplicitDifferenceMethod
    {
    public:
        ImplicitMethodUniformConcentration(
                std::vector<IterationInfo>* iterationsInfo,
                InitialParameters* initialParameters);

        ~ImplicitMethodUniformConcentration() override;

    protected:
        ImplicitMethodUniformConcentration();

        double calcLowerGamma(double I1, double I2) override;

        double calcUpperPhi(int i, double lowerGamma, double I1) override;
    };
}
