#pragma once

#include <vector>

#include <MagneticFluidFormAlgorithm/MagneticFluidFormAlgorithm.h>


namespace algorithm
{
    enum ConfigFileType
    {
        ConfigFileTypeNone,
        ConfigFileTypeJson
    };

    class AlgorithmConfigurator
    {
    public:
        std::vector<TargetParameter>* readAlgorithmSequenceFromFile(const char* filePath, ConfigFileType fileType);

        std::vector<TargetParameter>* getTutorialAlgorithmSequence();
    };
}
