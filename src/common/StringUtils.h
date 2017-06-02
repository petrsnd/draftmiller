#pragma once

// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>

namespace Magenta {

// Comparison Functions
int StringsCompare( const std::string& s1, const std::string& s2, bool ignoreCase = false );
bool StringsAreEqual( const std::string& s1, const std::string& s2, bool ignoreCase = false );
bool StringStartsWith( const std::string& s, const std::string& with, bool ignoreCase = false );
bool StringEndsWith( const std::string& s, const std::string& with, bool ignoreCase = false );
bool StringContains( const std::string& s, const std::string& contains, bool ignoreCase = false );

// Manipulation Functions
std::string ToLower( const std::string& s );
std::string ToUpper( const std::string& s );
std::string ReplaceAll( const std::string& s, const std::string& replace, const std::string& with, bool ignoreCase = false );
std::string TrimRight( const std::string& s );
std::string TrimLeft( const std::string& s );
std::string Trim( const std::string& s );

// Assorted Useful Functions
bool StringIsNumeric( const std::string& s );

} // Magenta

