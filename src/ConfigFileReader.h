#pragma once

#include <vector>

#include <MagneticFluidFormAlgorithm/MagneticFluidFormAlgorithm.h>

namespace algorithm
{
    const auto ALGORITHM_CONFIG_KEY = "algorithmConfig";
    const auto CALCULATIONS_SEQUENCE_KEY = "calculationsSequence";

    const auto PARAMETER_NAME_KEY = "parameterName";
    const auto TARGET_VALUE_KEY = "targetValue";
    const auto STEP_KEY = "step";
    const auto DRAW_RATE_KEY = "drawRate";

    class ConfigFileReader
    {
    public:
        virtual std::vector<TargetParameter>* readCalculationsSequenceFromFile(const char* filePath) = 0;
    };
}
