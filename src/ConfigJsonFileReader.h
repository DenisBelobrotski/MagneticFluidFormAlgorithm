#pragma once

#include <vector>

#include <MagneticFluidFormAlgorithm/ConfigFileReader.h>
#include <rapidjson/document.h>


namespace algorithm
{
    class ConfigJsonFileReader : public ConfigFileReader
    {
    private:
        rapidjson::Document* document;
    public:
        ConfigJsonFileReader();

        explicit ConfigJsonFileReader(const std::string& filePath);

        ~ConfigJsonFileReader() override;

        std::vector<TargetParameter>* readCalculationsSequenceFromFile() override;

        InitialParameters* readAlgorithmInitialParameters() override;
    };
}
