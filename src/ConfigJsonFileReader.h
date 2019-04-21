#pragma once

#include <vector>

#include "ConfigFileReader.h"


namespace algorithm
{
    class ConfigJsonFileReader : public ConfigFileReader
    {
        std::vector<TargetParameter>* readCalculationsSequenceFromFile(const char* filePath) override;
    };
}
