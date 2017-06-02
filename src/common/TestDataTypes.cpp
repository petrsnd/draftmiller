// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "DataBuffer.h"
#include "StringUtils.h"
#include "DataTypes.h"

#include "UnitTestSuite.h"

using namespace std;
using namespace Magenta;

// DataTypes.h
static void TestBoolToString()
{
    ASSERT_ARE_EQUAL( BoolToString( false ), "false" );
    ASSERT_ARE_EQUAL( BoolToString( true ), "true" );
}
static void TestDoubleToString()
{
    // using operator==() is pretty dubious
    ASSERT_ARE_EQUAL( DoubleToString( 0 ), "0.0000" );
    ASSERT_ARE_EQUAL( DoubleToString( 0.0 ), "0.0000" );
    ASSERT_ARE_EQUAL( DoubleToString( 0.0121 ), "0.0121" );
    ASSERT_ARE_EQUAL( DoubleToString( 10.01 ), "10.0100" );
    ASSERT_ARE_EQUAL( DoubleToString( 380.2938 ), "380.2938" );
    ASSERT_ARE_EQUAL( DoubleToString( -555.2736 ), "-555.2736" );
}
static void TestIntToString()
{
    ASSERT_ARE_EQUAL( IntToString( 0 ), "0" );
    ASSERT_ARE_EQUAL( IntToString( -1 ), "-1" );
    ASSERT_ARE_EQUAL( IntToString( 1 ), "1" );
    ASSERT_ARE_EQUAL( IntToString( 71382473 ), "71382473" );
    ASSERT_ARE_EQUAL( IntToString( -71382473 ), "-71382473" );
    ASSERT_ARE_EQUAL( IntToString( -2147483648 ), "-2147483648" );
    ASSERT_ARE_EQUAL( IntToString( 2147483647 ), "2147483647" );
}
static void TestStringToBool()
{
    ASSERT_TRUE( StringToBool( "true" ) );
    ASSERT_TRUE( StringToBool( "True" ) );
    ASSERT_TRUE( StringToBool( "TRUE" ) );
    ASSERT_TRUE( StringToBool( "TRuE" ) );
    ASSERT_TRUE( StringToBool( "\n\t  \rtrue \t\r\n  " ) );
    ASSERT_FALSE( StringToBool( "troo" ) );
    ASSERT_FALSE( StringToBool( "tr ue" ) );
    ASSERT_FALSE( StringToBool( "false" ) );
    ASSERT_FALSE( StringToBool( "False" ) );
    ASSERT_FALSE( StringToBool( "FALSE" ) );
    ASSERT_FALSE( StringToBool( "FalsE" ) );
    ASSERT_FALSE( StringToBool( "\n\t  \rFalsE\n\t  \r" ) );
}
static void TestStringToDouble()
{
    // using operator==() is pretty dubious
    ASSERT_ARE_EQUAL( StringToDouble( "0" ), 0.0000 );
    ASSERT_ARE_EQUAL( StringToDouble( "0.0" ), 0.0000 );
    ASSERT_ARE_EQUAL( StringToDouble( "0.0121" ), 0.0121 );
    ASSERT_ARE_EQUAL( StringToDouble( "10.01" ), 10.0100 );
    ASSERT_ARE_EQUAL( StringToDouble( "  380.2938  " ), 380.2938 );
    ASSERT_ARE_EQUAL( StringToDouble( "-555.2736" ), -555.2736 );
    ASSERT_THROWS( StringToDouble( "-55a5.2736" ), TypeConversionException );
    ASSERT_THROWS( StringToDouble( "-555.2.736" ), TypeConversionException );
}
static void TestStringToInt()
{
    ASSERT_ARE_EQUAL( StringToInt( "0" ), 0 );
    ASSERT_ARE_EQUAL( StringToInt( "-1" ), -1 );
    ASSERT_ARE_EQUAL( StringToInt( "1" ), 1 );
    ASSERT_ARE_EQUAL( StringToInt( "71382473" ), 71382473 );
    ASSERT_ARE_EQUAL( StringToInt( "-71382473" ), -71382473 );
    ASSERT_ARE_EQUAL( StringToInt( " \t \r\n71382473   \t" ), 71382473 );
    ASSERT_ARE_EQUAL( StringToInt( "\t\t\t-71382473\r\n" ), -71382473 );
    ASSERT_ARE_EQUAL( StringToInt( "-2147483648" ), -2147483648 );
    ASSERT_ARE_EQUAL( StringToInt( "2147483647" ), 2147483647 );
    // StringToInt does not round, it truncates
    ASSERT_ARE_EQUAL( StringToInt( "293801.928" ), 293801 );
    ASSERT_ARE_EQUAL( StringToInt( "293801.128" ), 293801 );
    ASSERT_THROWS( StringToInt( "7138a2473" ), TypeConversionException );
}


int main( int argc, char** argv )
{
    bool dataTypesAllPassed = false;
    DECLARE_UNIT_TEST_SUITE( DataTypes )
        ADD_UNIT_TEST( DataTypes, TestBoolToString )
        ADD_UNIT_TEST( DataTypes, TestDoubleToString )
        ADD_UNIT_TEST( DataTypes, TestIntToString )
        ADD_UNIT_TEST( DataTypes, TestStringToBool )
        ADD_UNIT_TEST( DataTypes, TestStringToDouble )
        ADD_UNIT_TEST( DataTypes, TestStringToInt )
    END_UNIT_TEST_SUITE( DataTypes, dataTypesAllPassed )

    return ( dataTypesAllPassed ? 0 : 1 );
}

