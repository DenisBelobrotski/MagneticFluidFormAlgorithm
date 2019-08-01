#include <MagneticFluidFormAlgorithm/ConfigJsonReader.h>

#include <fstream>
#include <sstream>

#include <rapidjson/document.h>
#include <MagneticFluidFormAlgorithm/Exceptions.h>


algorithm::ConfigJsonReader::ConfigJsonReader() : ConfigReader(), document(nullptr) {}


void algorithm::ConfigJsonReader::initializeJsonDocument(rapidjson::Document* parsedJsonDocument)
{
    document = parsedJsonDocument;

    if (!document->IsObject())
    {
        delete document;
        throw IncorrectConfigException("Json document does not contain any object");
    }

    if (!document->HasMember(ALGORITHM_CONFIG_KEY))
    {
        delete document;
        throw IncorrectConfigException("Json document does not contain algorithm config");
    }
}


algorithm::ConfigJsonReader::~ConfigJsonReader()
{
    delete document;
}


std::vector<algorithm::TargetParameter>* algorithm::ConfigJsonReader::getCalculationsSequence()
{
    auto resultSequence = new std::vector<algorithm::TargetParameter>();

    auto algorithmConfigObject = (*document)[ALGORITHM_CONFIG_KEY].GetObject();

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


std::vector<algorithm::TargetParameter>* algorithm::ConfigJsonReader::getTutorialCalculationsSequence()
{
    auto resultSequence = new std::vector<algorithm::TargetParameter>();

    return resultSequence;
}


algorithm::InitialParameters* algorithm::ConfigJsonReader::getAlgorithmInitialParameters()
{
    auto algorithmConfigObject = (*document)[ALGORITHM_CONFIG_KEY].GetObject();

    return new InitialParameters(
            algorithmConfigObject[NODES_NUMBER_KEY].GetInt(),
            algorithmConfigObject[TARGET_ACCURACY_KEY].GetDouble(),
            algorithmConfigObject[MAX_ITERATIONS_NUMBER_KEY].GetInt(),
            algorithmConfigObject[MIN_RELAXATION_PARAMETER_KEY].GetDouble(),
            algorithmConfigObject[RELAXATION_MULTIPLIER_KEY].GetDouble(),
            algorithmConfigObject[INITIAL_TAU_KEY].GetDouble(),
            algorithmConfigObject[INITIAL_U_KEY].GetDouble(),
            algorithmConfigObject[INITIAL_B0_KEY].GetDouble(),
            algorithmConfigObject[INITIAL_A1_KEY].GetDouble(),
            algorithmConfigObject[INITIAL_A2_KEY].GetDouble(),
            algorithmConfigObject[INITIAL_ALPHA_KEY].GetDouble()
    );
}


algorithm::InitialParameters* algorithm::ConfigJsonReader::getDefaultAlgorithmInitialParameters()
{
    return new InitialParameters(100, 1E-5, 10000, 0.05, 0.5, 1, 3, 1, 6, 0.05, M_PI_4);
}
