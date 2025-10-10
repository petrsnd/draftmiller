// (c) 2025 petrsnd@secureidx.com.  All rights reserved.

#include "DataTypes.h"

#include "StringUtils.h"

namespace DraftMiller {

TypeConversionException::TypeConversionException(const std::string& from,
    const std::string& to, const std::string& msg)
    : runtime_error(SC() << "Unable to convert " << from << " to " << to << " - " << msg)
{}

TypeConversionException::~TypeConversionException() = default;

std::string BoolToString(const bool b)
{
    return (b ? "true" : "false");
}
std::string DoubleToString(const double d, const unsigned int precision)
{
    std::stringstream ss;
    ss.flags(std::ios_base::fixed);
    ss.precision(precision);
    ss << d;
    return ss.str();
}
std::string IntToString(const int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}
bool StringToBool(const std::string& s)
{
    // Anything but "true" returns false
    const std::string tmp = Trim(s);
    return ToLower(tmp) == "true";
}
double StringToDouble(const std::string& s)
{
    if (!StringIsNumeric(s))
    {
        throw TypeConversionException("string", "double", "'" + s + "' is not numeric");
    }
    std::stringstream ss(s);
    double d;
    ss >> d;
    return d;
}
int StringToInt(const std::string& s)
{
    if (!StringIsNumeric(s))
    {
        throw TypeConversionException("string", "int", "'" + s + "' is not numeric");
    }
    std::stringstream ss(s);
    int i;
    ss >> i;
    return i;
}

} // Magenta

