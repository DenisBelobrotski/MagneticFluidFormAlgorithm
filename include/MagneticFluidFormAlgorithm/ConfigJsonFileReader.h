#pragma once

#include "ConfigJsonReader.h"


namespace algorithm
{
    class ConfigJsonFileReader : public ConfigJsonReader
    {
    public:
        ConfigJsonFileReader() = default;

        explicit ConfigJsonFileReader(const std::string& filePath);
    };
}
