#pragma once

// (c) 2025 petrsnd@secureidx.com.  All rights reserved.

#include "Exception.h"
#include "DataBuffer.h"

#include <string>
#include <vector>
#include <set>

namespace DraftMiller {

// Types
typedef std::vector<char*> CharArrayList;

typedef std::vector<std::string> StringList;
typedef std::set<std::string> StringSet;

typedef std::vector<int> IntList;
typedef std::set<int> IntSet;

typedef std::vector<DataBuffer> DataBufferList;

// Exceptions
class TypeConversionException final : public std::runtime_error
{
public:
    TypeConversionException(const std::string& from, const std::string& to, const std::string& msg);
    ~TypeConversionException() override;
};

// Conversion Utilities
std::string BoolToString(bool b);
std::string DoubleToString(double d, unsigned int precision = 4);
std::string IntToString(int i);

bool StringToBool(const std::string& s);
double StringToDouble(const std::string& s);
int StringToInt(const std::string& s);

// List Conversion Utilities

// TODO:

} // Magenta
