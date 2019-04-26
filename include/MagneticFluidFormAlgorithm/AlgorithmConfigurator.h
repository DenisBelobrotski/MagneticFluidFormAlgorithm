#pragma once

#include "ConfigFileReader.h"

#include <vector>
#include <string>


namespace algorithm
{
    enum ConfigFileType
    {
        ConfigFileTypeNone,
        ConfigFileTypeJson
    };

    class AlgorithmConfigurator
    {
    private:
        ConfigFileReader* configFileReader;
    public:
        AlgorithmConfigurator();

        AlgorithmConfigurator(const std::string& filePath, ConfigFileType fileType);

        ~AlgorithmConfigurator();

        std::vector<TargetParameter>* readAlgorithmSequenceFromFile();

        std::vector<TargetParameter>* getTutorialAlgorithmSequence();

        InitialParameters* readAlgorithmInitialParameters();

        InitialParameters* getDefaultAlgorithmInitialParameters();
    };
}
