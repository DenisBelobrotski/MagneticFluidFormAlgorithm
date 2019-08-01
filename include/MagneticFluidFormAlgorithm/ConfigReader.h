#pragma once

#include <vector>

#include <MagneticFluidFormAlgorithm/DifferenceMethod.h>

namespace algorithm
{
    const auto ALGORITHM_CONFIG_KEY = "algorithmConfig";
    const auto CALCULATIONS_SEQUENCE_KEY = "calculationsSequence";

    const auto PARAMETER_NAME_KEY = "parameterName";
    const auto TARGET_VALUE_KEY = "targetValue";
    const auto STEP_KEY = "step";
    const auto DRAW_RATE_KEY = "drawRate";

    const auto NODES_NUMBER_KEY = "nodesNumber";
    const auto TARGET_ACCURACY_KEY = "targetAccuracy";
    const auto MAX_ITERATIONS_NUMBER_KEY = "maxIterationsNumber";
    const auto MIN_RELAXATION_PARAMETER_KEY = "minRelaxationParameter";
    const auto RELAXATION_MULTIPLIER_KEY = "relaxationMultiplier";
    const auto INITIAL_TAU_KEY = "initialTAU";
    const auto INITIAL_U_KEY = "initialU";
    const auto INITIAL_B0_KEY = "initialB0";
    const auto INITIAL_A1_KEY = "initialA1";
    const auto INITIAL_A2_KEY = "initialA2";
    const auto INITIAL_ALPHA_KEY = "initialALPHA";

    class ConfigReader
    {
    public:
        ConfigReader() = default;

        virtual ~ConfigReader() = default;

        virtual std::vector<TargetParameter>* getCalculationsSequence() = 0;

        virtual std::vector<TargetParameter>* getTutorialCalculationsSequence() = 0;

        virtual InitialParameters* getAlgorithmInitialParameters() = 0;

        virtual InitialParameters* getDefaultAlgorithmInitialParameters() = 0;
    };
}
