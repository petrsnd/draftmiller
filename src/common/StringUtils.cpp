// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "StringUtils.h"

#include <algorithm>
#include <functional>

namespace DraftMiller {

struct EqualToNoCaseChar : std::binary_function< char, char, bool >
{
    bool operator() ( const char& c1, const char& c2 ) const
    {
        return tolower( c1 ) == tolower( c2 );
    }
};

static std::function< bool ( char, char ) > GetComparison( bool ignoreCase )
{
    if ( ignoreCase )
    {
        return EqualToNoCaseChar();
    }
    else
    {
        return std::equal_to< char >();
    }
}

int StringsCompare( const std::string& s1, const std::string& s2, bool ignoreCase )
{
    if ( ignoreCase )
    {
        return ( ToLower( s1 ) ).compare( ToLower( s2 ) );
    }
    else
    {
        return s1.compare( s2 );
    }
}
bool StringsAreEqual( const std::string& s1, const std::string& s2, bool ignoreCase )
{
    return StringsCompare( s1, s2, ignoreCase ) == 0;
}
bool StringStartsWith( const std::string& s, const std::string& with, bool ignoreCase )
{
    if ( with.length() > s.length() )
    {
        return false;
    }
    std::function< bool ( char, char ) > cmp = GetComparison( ignoreCase );
    for ( size_t i = 0; i < with.length(); i++ )
    {
        if ( ! cmp( s[i], with[i] ) )
        {
            return false;
        }
    }
    return true;
}
bool StringEndsWith( const std::string& s, const std::string& with, bool ignoreCase )
{
    if ( with.length() > s.length() )
    {
        return false;
    }
    std::function< bool ( char, char ) > cmp = GetComparison( ignoreCase );
    size_t start = s.length() - with.length();
    for ( size_t i = 0; i < with.length(); i++ )
    {
        if ( ! cmp( s[start + i], with[i] ) )
        {
            return false;
        }
    }
    return true;
}
bool StringContains( const std::string& s, const std::string& contains, bool ignoreCase )
{
    if ( contains.length() > s.length() )
    {
        return false;
    }
    if ( ignoreCase )
    {
        return ToLower( s ).find( ToLower( contains ) ) != std::string::npos;
    }
    else
    {
        return s.find( contains ) != std::string::npos;
    }
}
std::string ToLower( const std::string& s )
{
    std::string ret;
    ret.reserve( s.length() );
    for ( std::string::const_iterator it = s.begin(); it != s.end(); it++ )
    {
        ret.append( 1, ( char )tolower( *it ) );
    }
    return ret;
}
std::string ToUpper( const std::string& s )
{
    std::string ret;
    ret.reserve( s.length() );
    for ( std::string::const_iterator it = s.begin(); it != s.end(); it++ )
    {
        ret.append( 1, ( char )toupper( *it ) );
    }
    return ret;
}
static std::string::iterator FindAndReplace_Internal( std::string& str,
                                                      size_t offset,
                                                      const std::string& replace,
                                                      const std::string& with,
                                                      bool ignoreCase )
{
    if ( str.begin() + offset >= str.end() )
    {
        return str.end();
    }
    std::string::iterator occurrence;
    std::function< bool ( char, char ) > cmp = GetComparison( ignoreCase );
    occurrence = std::search( str.begin() + offset, str.end(), replace.begin(), replace.end(), cmp );
    if ( occurrence >= str.end() )
    {
        return str.end();
    }
    size_t pos = ( size_t )( occurrence - str.begin() );
    str.replace( occurrence, occurrence + replace.size(), with.begin(), with.end() );
    return str.begin() + pos + with.size();
}
std::string ReplaceAll( const std::string& s, const std::string& replace, const std::string& with, bool ignoreCase )
{
    if ( replace.empty() )
    {
        if ( s.empty() )
        {
            return with;
        }
        return s;
    }
    std::string tmp = s;
    std::string::iterator start = tmp.begin();
    while ( true )
    {
        start = FindAndReplace_Internal( tmp, ( size_t )( start - tmp.begin() ), replace, with, ignoreCase );
        if ( start == tmp.end() )
        {
            break;
        }
    }
    return tmp;
}
std::string TrimRight( const std::string& s )
{
    std::string ret;
    std::string::size_type pos = s.find_last_not_of( " \t\r\n" );
    if ( pos != std::string::npos )
    {
        ret = s.substr( 0, pos + 1 );
    }
    return ret;
}
std::string TrimLeft( const std::string& s )
{
    std::string ret;
    std::string::size_type pos = s.find_first_not_of( " \t\r\n" );
    if ( pos != std::string::npos )
    {
        ret = s.substr( pos );
    }
    return ret;
}
std::string Trim( const std::string& s )
{
    return TrimLeft( TrimRight( s ) );
}
bool StringIsNumeric( const std::string& s )
{
    if ( s.empty() )
    {
        return false;
    }
    std::string tmp = Trim( s );
    std::string::iterator it = tmp.begin();
    if ( *it == '-' )
    {
        it++;
    }
    bool foundDecimal = false;
    for ( ; it != tmp.end(); it++ )
    {
        if ( ! isdigit( *it ) )
        {
            if ( foundDecimal )
            {
                return false;
            }
            else
            {
                if ( *it == '.' )
                {
                    foundDecimal = true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
    return true;
}

} // Magenta

