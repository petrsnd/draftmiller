#pragma once

// (c) 2025 petrsnd@secureidx.com.  All rights reserved.

#include <stdexcept>
#include <string>

namespace DraftMiller {

class Exception : public std::runtime_error
{
public:
    explicit Exception(const std::string &msg) : runtime_error(msg)
    {}
    ~Exception() override = default;
protected:
    Exception() : runtime_error("")
    {}
};

class NotImplementedException final : public Exception
{
public:
    explicit NotImplementedException() : Exception("Not Implemented")
    {}
    ~NotImplementedException() override = default;
};

} // Magenta
