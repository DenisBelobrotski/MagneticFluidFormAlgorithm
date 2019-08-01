#include <MagneticFluidFormAlgorithm/ConfigJsonStringReader.h>

#include <rapidjson/document.h>


algorithm::ConfigJsonStringReader::ConfigJsonStringReader(const std::string& jsonString)
{
    auto* jsonDocument = new rapidjson::Document();
    jsonDocument->Parse(jsonString.c_str());

    initializeJsonDocument(jsonDocument);
}
