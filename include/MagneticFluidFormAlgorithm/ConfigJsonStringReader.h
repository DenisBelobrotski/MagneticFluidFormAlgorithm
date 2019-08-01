#pragma once

#include "ConfigJsonReader.h"


namespace algorithm
{
    class ConfigJsonStringReader : public ConfigJsonReader
    {
    public:
        ConfigJsonStringReader() = default;

        explicit ConfigJsonStringReader(const std::string& jsonString);
    };
}
