#pragma once

// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "Exception.h"
#include "DataBuffer.h"

#include <string>
#include <vector>
#include <set>

namespace DraftMiller {

// Types
typedef std::vector< char* > CharArrayList;

typedef std::vector< std::string > StringList;
typedef std::set< std::string > StringSet;

typedef std::vector< int > IntList;
typedef std::set< int > IntSet;

typedef std::vector< DataBuffer > DataBufferList;

// Exceptions
class TypeConversionException : public Exception
{
public:
    TypeConversionException( const std::string& from,
                             const std::string& to,
                             const std::string& msg );
    virtual ~TypeConversionException();
};

// Conversion Utilities
std::string BoolToString( const bool b );
std::string DoubleToString( const double d, unsigned int precision = 4 );
std::string IntToString( const int i );

bool StringToBool( const std::string& s );
double StringToDouble( const std::string& s );
int StringToInt( const std::string& s );

// List Conversion Utilities

// TODO:

} // Magenta

