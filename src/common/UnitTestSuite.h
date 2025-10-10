#pragma once

// (c) 2025 petrsnd@secureidx.com.  All rights reserved.

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "Exception.h"
#include "StringUtils.h"

#include <string>
#include <utility>
#include <vector>
#include <iostream>

#include <functional>


namespace DraftMiller {

class UnitTestException final : public std::runtime_error
{
public:
    explicit UnitTestException(const std::string& errmsg) : std::runtime_error(errmsg)
    {}
    ~UnitTestException() override = default;
};

class UnitTestResult
{
public:
    explicit UnitTestResult(const bool passed = true, std::string  errmsg = "") :
        m_passed(passed), m_errmsg(std::move(errmsg))
    {}
    virtual ~UnitTestResult() = default;

    bool Passed() const
    {
        return m_passed;
    }
    std::string ErrorMessage() const
    {
        return m_errmsg;
    }
protected:
    bool m_passed;
    std::string m_errmsg;
};

class UnitTest
{
public:
    typedef std::function< void () > func_t;
    UnitTest(std::string name, func_t func) : m_name(std::move(name)), m_func(std::move(func))
    {}
    virtual ~UnitTest() = default;

    std::string Name() const
    {
        return m_name;
    }
    UnitTestResult Execute() const
    {
        try
        {
            m_func();
            return UnitTestResult();
        }
        catch (const UnitTestException& e)
        {
            return UnitTestResult(false, e.what());
        }
        catch (const NotImplementedException& e)
        {
            return UnitTestResult(false, e.what());
        }
        catch (const Exception& e)
        {
            return UnitTestResult(false, SC() << "Unexpected exception: " << e.what());
        }
        catch (...)
        {
            return UnitTestResult(false, "Unhandled exception");
        }
    }
protected:
    std::string m_name;
    func_t m_func;
};
typedef std::vector< UnitTest > UnitTestList;

class UnitTestSuite
{
public:
    explicit UnitTestSuite(std::string name) : m_name(std::move(name))
    {}
    virtual ~UnitTestSuite() = default;

    void RegisterTest(const UnitTest& t)
    {
        m_unitTests.push_back(t);
    }
    bool ExecuteTests() const
    {
        std::cout << "Executing UnitTestSuite: " << m_name << std::endl;
        std::cout << "Total Tests: " << m_unitTests.size() << std::endl;
        size_t totalPassed = 0;
        for (const auto& m_unitTest : m_unitTests)
        {
            std::cout << "    " << m_unitTest.Name() << " : " << std::flush;
            UnitTestResult r = m_unitTest.Execute();
            if (r.Passed())
            {
                std::cout << "PASSED" << std::endl;
                totalPassed++;
            }
            else
            {
                std::cout << "FAILED" << std::endl;
                std::cout << "      Error was: " << r.ErrorMessage() << std::endl;
            }
        }
        std::cout << "Results: " << totalPassed << " of " << m_unitTests.size() << " passed" << std::endl << std::endl;
        return (m_unitTests.size() - totalPassed) == 0;
    }
protected:
    std::string m_name;
    UnitTestList m_unitTests;
};

#define DECLARE_UNIT_TEST_SUITE(name) UnitTestSuite testsuite##name(#name);
#define ADD_UNIT_TEST(name, test) testsuite##name.RegisterTest(UnitTest(#test, &test));
#define END_UNIT_TEST_SUITE(name, ret) ret = testsuite##name.ExecuteTests();

#define FAIL_TEST(msg) \
    throw UnitTestException(msg)
#define ASSERT_TRUE(expr) if (! (expr)) \
    throw UnitTestException("Assertion failed: (" + std::string(#expr) + ") did not evaluate to true.");
#define ASSERT_FALSE(expr) if (expr) \
    throw UnitTestException("Assertion failed: (" + std::string(#expr) + ") should evaluate to false.");
#define ASSERT_ARE_EQUAL(expr1, expr2) if (! ((expr1) == (expr2))) \
    throw UnitTestException("Assertion failed: (" + std::string(#expr1) + ") is not equal to (" \
    + std::string(#expr2) + "), i.e. operator==() returned false");
#define ASSERT_ARE_NOT_EQUAL(expr1, expr2) if (! ((expr1) != (expr2))) \
    throw UnitTestException("Assertion failed: (" + std::string(#expr1) + ") is *NOT* not equal to (" \
    + std::string(#expr2) + "), i.e. operator!=() returned false");
#define ASSERT_THROWS(expr, excptn) try { expr; \
    throw UnitTestException("Assertion failed: (" + std::string(#expr) + ") did not throw an exception, expected " \
    + std::string(#excptn)); } catch (const excptn&) {}

} // Magenta

