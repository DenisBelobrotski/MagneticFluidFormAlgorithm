#include "ConfigJsonFileReader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <rapidjson/document.h>
#include <MagneticFluidFormAlgorithm/Exceptions.h>


std::vector<algorithm::TargetParameter>* algorithm::ConfigJsonFileReader::readCalculationsSequenceFromFile(
        const char* filePath)
{
    auto resultSequence = new std::vector<algorithm::TargetParameter>();

    std::ifstream configFile(filePath);
    std::stringstream jsonBuffer;
    jsonBuffer << configFile.rdbuf();

    rapidjson::Document document;
    document.Parse(jsonBuffer.str().c_str());

    if (!document.IsObject())
    {
        throw IncorrectConfigException("Json document does not contain any object");
    }

    if (!document.HasMember(ALGORITHM_CONFIG_KEY))
    {
        throw IncorrectConfigException("Json document does not contain algorithm config");
    }

    auto algorithmConfigObject = document[ALGORITHM_CONFIG_KEY].GetObject();

    if (!algorithmConfigObject.HasMember(CALCULATIONS_SEQUENCE_KEY))
    {
        throw IncorrectConfigException("Json document does not contain algorithm calculations sequence");
    }

    auto calculationsSequence = algorithmConfigObject[CALCULATIONS_SEQUENCE_KEY].GetArray();

    for (auto& jsonTargetParameter : calculationsSequence)
    {
        TargetParameter targetParameter;
        targetParameter.parameterName = jsonTargetParameter[PARAMETER_NAME_KEY].GetString();
        targetParameter.targetValue = jsonTargetParameter[TARGET_VALUE_KEY].GetDouble();
        targetParameter.step = jsonTargetParameter[STEP_KEY].GetDouble();
        targetParameter.drawRate = jsonTargetParameter[DRAW_RATE_KEY].GetInt();

        resultSequence->push_back(targetParameter);
    }

    return resultSequence;
}
