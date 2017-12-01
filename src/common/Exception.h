#pragma once

// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>

namespace DraftMiller {

class IException
{
public:
    virtual std::string What() const = 0;
};
    
class Exception : public IException
{
public:
    Exception( const std::string& msg ) : m_msg( msg )
    {}
    virtual ~Exception()
    {}
    virtual std::string What() const
    {
        return m_msg;
    }
protected:
    Exception() : m_msg( "" )
    {}
    std::string m_msg;
};

class NotImplementedException : public Exception
{
public:
    NotImplementedException() : Exception( "Not Implemented" )
    {}
    virtual ~NotImplementedException()
    {}
};

} // Magenta

