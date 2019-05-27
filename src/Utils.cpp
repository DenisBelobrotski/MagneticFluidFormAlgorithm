#include "MagneticFluidFormAlgorithm/Utils.h"


double algorithm::IterationInfo::getDoubleValueByName(const std::string& valueName)
{
    double result = 0;

    if (valueName == "U")
    {
        result = variables.U;
    }
    if (valueName == "B0")
    {
        result = variables.B0;
    }
    if (valueName == "A1")
    {
        result = variables.A1;
    }
    if (valueName == "A2")
    {
        result = variables.A2;
    }
    if (valueName == "ALPHA")
    {
        result = variables.ALPHA;
    }

    return result;
}


double algorithm::IterationInfo::getMainDoubleValue()
{
    return getDoubleValueByName(mainValueName);
}
