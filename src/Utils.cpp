#include "MagneticFluidFormAlgorithm/Utils.h"


double algorithm::IterationInfo::getDoubleValueByName(const std::string& valueName)
{
    double result = 0;

    if (valueName == "U")
    {
        result = u;
    }
    if (valueName == "B0")
    {
        result = b0;
    }
    if (valueName == "A1")
    {
        result = a1;
    }
    if (valueName == "A2")
    {
        result = a2;
    }
    if (valueName == "ALPHA")
    {
        result = alpha;
    }

    return result;
}


double algorithm::IterationInfo::getMainDoubleValue()
{
    return getDoubleValueByName(mainValueName);
}
