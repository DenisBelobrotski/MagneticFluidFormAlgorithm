#include "MagneticFluidFormAlgorithm/AlgorithmConfigurator.h"
#include "ConfigJsonFileReader.h"


algorithm::AlgorithmConfigurator::AlgorithmConfigurator() : configFileReader(nullptr) {}

algorithm::AlgorithmConfigurator::AlgorithmConfigurator(const std::string& filePath, ConfigFileType fileType)
{
    switch (fileType)
    {
        case ConfigFileTypeJson:
            configFileReader = new ConfigJsonFileReader(filePath);
            break;

        default:
            configFileReader = nullptr;
    }
}

algorithm::AlgorithmConfigurator::~AlgorithmConfigurator()
{
    delete configFileReader;
}


std::vector<algorithm::TargetParameter>* algorithm::AlgorithmConfigurator::readAlgorithmSequenceFromFile()
{
    return configFileReader->readCalculationsSequenceFromFile();
}


std::vector<algorithm::TargetParameter>* algorithm::AlgorithmConfigurator::getTutorialAlgorithmSequence()
{
    auto resultSequence = new std::vector<algorithm::TargetParameter>();

    return resultSequence;
}


algorithm::InitialParameters* algorithm::AlgorithmConfigurator::readAlgorithmInitialParameters()
{
    return configFileReader->readAlgorithmInitialParameters();
}


algorithm::InitialParameters* algorithm::AlgorithmConfigurator::getDefaultAlgorithmInitialParameters()
{
    return new InitialParameters(100, 1E-5, 10000, 0.05, 0.5, 1, 3, 1, 6, 0.05, M_PI_4);
}
