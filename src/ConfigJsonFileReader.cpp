#include <MagneticFluidFormAlgorithm/ConfigJsonFileReader.h>

#include <fstream>
#include <sstream>

#include <rapidjson/document.h>


algorithm::ConfigJsonFileReader::ConfigJsonFileReader(const std::string& filePath)
{
    std::ifstream configFile(filePath);
    std::stringstream jsonBuffer;
    jsonBuffer << configFile.rdbuf();

    auto* jsonDocument = new rapidjson::Document();
    jsonDocument->Parse(jsonBuffer.str().c_str());

    initializeJsonDocument(jsonDocument);
}
