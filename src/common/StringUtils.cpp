// (c) 2025 petrsnd@secureidx.com.  All rights reserved.

#include "StringUtils.h"

#include <algorithm>
#include <functional>

namespace DraftMiller {

static std::function<bool (char, char)> GetComparison(const bool ignoreCase)
{
    if (ignoreCase)
    {
        return [](const char a, const char b) {
            return std::tolower(a) == std::tolower(b);
        };
    }

    return [](const char a, const char b) {
        return a == b;
    };
}

int StringsCompare(const std::string& s1, const std::string& s2, const bool ignoreCase)
{
    if (ignoreCase)
    {
        return ToLower(s1).compare(ToLower(s2));
    }

    return s1.compare(s2);
}
bool StringsAreEqual(const std::string& s1, const std::string& s2, const bool ignoreCase)
{
    return StringsCompare(s1, s2, ignoreCase) == 0;
}
bool StringStartsWith(const std::string& s, const std::string& with, const bool ignoreCase)
{
    if (with.length()> s.length())
    {
        return false;
    }

    const auto cmp = GetComparison(ignoreCase);
    for (size_t i = 0; i <with.length(); i++)
    {
        if (!cmp(s[i], with[i]))
        {
            return false;
        }
    }

    return true;
}
bool StringEndsWith(const std::string& s, const std::string& with, const bool ignoreCase)
{
    if (with.length()> s.length())
    {
        return false;
    }

    const auto cmp = GetComparison(ignoreCase);
    const size_t start = s.length() - with.length();
    for (size_t i = 0; i <with.length(); i++)
    {
        if (!cmp(s[start + i], with[i]))
        {
            return false;
        }
    }

    return true;
}
bool StringContains(const std::string& s, const std::string& contains, const bool ignoreCase)
{
    if (contains.length()> s.length())
    {
        return false;
    }

    if (ignoreCase)
    {
        return ToLower(s).find(ToLower(contains)) != std::string::npos;
    }

    return s.find(contains) != std::string::npos;
}
std::string ToLower(const std::string& s)
{
    std::string ret;
    ret.reserve(s.length());
    for (const char it : s)
    {
        ret.append(1, static_cast<char>(std::tolower(it)));
    }

    return ret;
}
std::string ToUpper(const std::string& s)
{
    std::string ret;
    ret.reserve(s.length());
    for (const char it : s)
    {
        ret.append(1, static_cast<char>(std::toupper(it)));
    }

    return ret;
}
static std::string::iterator FindAndReplace_Internal(std::string& str, const std::string::difference_type offset,
    const std::string& replace, const std::string& with, const bool ignoreCase)
{
    if (str.begin() + offset >= str.end())
    {
        return str.end();
    }

    const auto cmp = GetComparison(ignoreCase);
    const auto occurrence =
        std::search(str.begin() + offset, str.end(), replace.begin(), replace.end(), cmp);
    if (occurrence >= str.end())
    {
        return str.end();
    }

    const auto pos = occurrence - str.begin();
    str.replace(occurrence, occurrence + static_cast<std::string::difference_type>(replace.size()),
        with.begin(), with.end());

    return str.begin() + pos + static_cast<std::string::difference_type>(with.size());
}
std::string ReplaceAll(const std::string& s, const std::string& replace, const std::string& with, const bool ignoreCase)
{
    if (replace.empty())
    {
        if (s.empty())
        {
            return with;
        }

        return s;
    }

    std::string tmp = s;
    auto start = tmp.begin();
    while (true)
    {
        start = FindAndReplace_Internal(tmp, start - tmp.begin(), replace, with, ignoreCase);
        if (start == tmp.end())
        {
            break;
        }
    }

    return tmp;
}
std::string TrimRight(const std::string& s)
{
    std::string ret;
    std::string::size_type pos = s.find_last_not_of(" \t\r\n");
    if (pos != std::string::npos)
    {
        ret = s.substr(0, pos + 1);
    }

    return ret;
}
std::string TrimLeft(const std::string& s)
{
    std::string ret;
    std::string::size_type pos = s.find_first_not_of(" \t\r\n");
    if (pos != std::string::npos)
    {
        ret = s.substr(pos);
    }
    return ret;
}
std::string Trim(const std::string& s)
{
    return TrimLeft(TrimRight(s));
}
bool StringIsNumeric(const std::string& s)
{
    if (s.empty())
    {
        return false;
    }

    std::string tmp = Trim(s);
    auto it = tmp.begin();
    if (*it == '-')
    {
        ++it;
    }

    bool foundDecimal = false;
    for (; it != tmp.end(); ++it)
    {
        if (!isdigit(*it))
        {
            if (foundDecimal)
            {
                return false;
            }

            if (*it == '.')
            {
                foundDecimal = true;
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

} // Magenta
