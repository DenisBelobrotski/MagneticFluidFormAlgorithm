#include "MagneticFluidFormAlgorithm/AlgorithmConfigurator.h"

#include "ConfigFileReader.h"
#include "ConfigJsonFileReader.h"


std::vector<algorithm::TargetParameter>* algorithm::AlgorithmConfigurator::readAlgorithmSequenceFromFile(
        const char* filePath, ConfigFileType fileType)
{
    ConfigFileReader* configFileReader = nullptr;

    switch (fileType)
    {
        case ConfigFileTypeJson:
            configFileReader = new ConfigJsonFileReader();
            break;

        default:
            configFileReader = nullptr;
    }

    return configFileReader->readCalculationsSequenceFromFile(filePath);
}


std::vector<algorithm::TargetParameter>* algorithm::AlgorithmConfigurator::getTutorialAlgorithmSequence()
{
    auto resultSequence = new std::vector<algorithm::TargetParameter>();

    return resultSequence;
}
