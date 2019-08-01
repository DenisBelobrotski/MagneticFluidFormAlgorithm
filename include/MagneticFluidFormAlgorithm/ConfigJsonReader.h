#pragma once

#include <vector>

#include <MagneticFluidFormAlgorithm/ConfigReader.h>
#include <rapidjson/document.h>


namespace algorithm
{
    class ConfigJsonReader : public ConfigReader
    {
    private:
        rapidjson::Document* document;

    public:
        ConfigJsonReader();

        ~ConfigJsonReader() override;

        std::vector<TargetParameter>* getCalculationsSequence() override;

        std::vector<TargetParameter>* getTutorialCalculationsSequence() override;

        InitialParameters* getAlgorithmInitialParameters() override;

        InitialParameters* getDefaultAlgorithmInitialParameters() override;

    protected:
        void initializeJsonDocument(rapidjson::Document* parsedJsonDocument);
    };
}
