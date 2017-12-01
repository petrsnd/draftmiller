#pragma once

// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "Exception.h"
#include "StringUtils.h"

#include <string>
#include <vector>
#include <iostream>

#include <functional>


namespace DraftMiller {

class UnitTestException
{
public:
    UnitTestException( std::string errmsg ) : m_errmsg( errmsg )
    {}
    ~UnitTestException()
    {}
    std::string ErrorMessage() const
    {
        return m_errmsg;
    }
private:
    std::string m_errmsg;
};

class UnitTestResult
{
public:
    UnitTestResult( const bool passed = true, const std::string& errmsg = "" ) : m_passed( passed ), m_errmsg( errmsg )
    {}
    virtual ~UnitTestResult()
    {}

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
    UnitTest( const std::string& name, func_t func ) : m_name( name ), m_func( func )
    {}
    virtual ~UnitTest()
    {}

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
        catch ( const UnitTestException& e )
        {
            return UnitTestResult( false, e.ErrorMessage() );
        }
        catch ( const NotImplementedException& e )
        {
            return UnitTestResult( false, e.What() );
        }
        catch ( const Exception& e )
        {
            return UnitTestResult( false, SC() << "Unexpected exception: " << e.What() );
        }
        catch ( ... )
        {
            return UnitTestResult( false, "Unhandled exception" );
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
    UnitTestSuite( const std::string& name ) : m_name( name )
    {}
    virtual ~UnitTestSuite()
    {}

    void RegisterTest( const UnitTest& t )
    {
        m_unitTests.push_back( t );
    }
    bool ExecuteTests() const
    {
        std::cout << "Executing UnitTestSuite: " << m_name << std::endl;
        std::cout << "Total Tests: " << m_unitTests.size() << std::endl;
        size_t totalPassed = 0;
        for ( UnitTestList::const_iterator it = m_unitTests.begin(); it != m_unitTests.end(); it++ )
        {
            std::cout << "    " << it->Name() << " : " << std::flush;
            UnitTestResult r = it->Execute();
            if ( r.Passed() )
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
        return ( m_unitTests.size() - totalPassed ) == 0;
    }
protected:
    std::string m_name;
    UnitTestList m_unitTests;
};

#define DECLARE_UNIT_TEST_SUITE( name ) UnitTestSuite testsuite##name( #name );
#define ADD_UNIT_TEST( name, test ) testsuite##name.RegisterTest( UnitTest( #test, &test ) );
#define END_UNIT_TEST_SUITE( name, ret ) ret = testsuite##name.ExecuteTests();

#define FAIL_TEST( msg ) \
    throw UnitTestException( msg )
#define ASSERT_TRUE( expr ) if ( ! ( expr ) ) \
    throw UnitTestException( "Assertion failed: ( " + std::string( #expr ) + " ) did not evaluate to true." );
#define ASSERT_FALSE( expr ) if ( expr ) \
    throw UnitTestException( "Assertion failed: ( " + std::string( #expr ) + " ) should evaluate to false." );
#define ASSERT_ARE_EQUAL( expr1, expr2 ) if ( ! ( ( expr1 ) == ( expr2 ) ) ) \
    throw UnitTestException( "Assertion failed: ( " + std::string( #expr1 ) + " ) is not equal to ( " \
    + std::string( #expr2 ) + " ), i.e. operator==() returned false" );
#define ASSERT_ARE_NOT_EQUAL( expr1, expr2 ) if ( ! ( ( expr1 ) != ( expr2 ) ) ) \
    throw UnitTestException( "Assertion failed: ( " + std::string( #expr1 ) + " ) is *NOT* not equal to ( " \
    + std::string( #expr2 ) + " ), i.e. operator!=() returned false" );
#define ASSERT_THROWS( expr, excptn ) try { expr; \
    throw UnitTestException( "Assertion failed: ( " + std::string( #expr ) + " ) did not throw an exception, expected " \
    + std::string( #excptn ) ); } catch ( const excptn ) {}

} // Magenta

