// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "DataBuffer.h"
#include "StringUtils.h"

#include "UnitTestSuite.h"

using namespace Magenta;

// StringUtils.h
static void TestStringsCompare()
{
    ASSERT_TRUE( StringsCompare( "", "", false ) == 0 );
    ASSERT_TRUE( StringsCompare( "", "", true ) == 0 );
    ASSERT_FALSE( StringsCompare( "this doesn't match.", "this part at all." ) == 0 );
    ASSERT_FALSE( StringsCompare( "asdf", "asdfa" ) == 0 );
    ASSERT_FALSE( StringsCompare( "aasdf", "asdf" ) == 0 );
    ASSERT_TRUE( StringsCompare( "this is some baloney text", "this is some baloney text", false ) == 0 );
    ASSERT_FALSE( StringsCompare( "this is some baloney text", "this is some BaLoNeY text", false ) == 0 );
    ASSERT_TRUE( StringsCompare( "this is some baloney text", "this is some BaLoNeY text", true ) == 0 );
    ASSERT_TRUE( StringsCompare( "abcdefg12384", "zbcdeksl1929" ) < 0 );
    ASSERT_TRUE( StringsCompare( "asdfjdksl", "12930984" ) > 0 );
}
static void TestStringsAreEqual()
{
    ASSERT_TRUE( StringsAreEqual( "", "", false ) );
    ASSERT_TRUE( StringsAreEqual( "", "", true ) );
    ASSERT_FALSE( StringsAreEqual( "", "fruit stripe gum is delicious" ) );
    ASSERT_FALSE( StringsAreEqual( "apples are for eatING\n\r\taa\v", "" ) );
    ASSERT_FALSE( StringsAreEqual( "what a difference a day makes", "fruit stripe gum is delicious" ) );
    ASSERT_FALSE( StringsAreEqual( "12343234@3$@3$@#4213@$@3$2#41@#4!234", ";alskjd f.xc/>vasldkjf;asdf/,.c" ) );
    ASSERT_TRUE( StringsAreEqual( "thies is923 aex.altiy the same 2309texT ASD the\nother", "thies is923 aex.altiy the same 2309texT ASD the\nother", false ) );
    ASSERT_TRUE( StringsAreEqual( "apples are for eatING\n\r\taa\v", "apples are for eatING\n\r\taa\v" ) );
    ASSERT_FALSE( StringsAreEqual( "this differs only by case\n", "THis DiffeRS OnlY bY CaSE\n", false ) );
    ASSERT_TRUE( StringsAreEqual( "this differs only by case\n", "THis DiffeRS OnlY bY CaSE\n", true ) );
}
static void TestStringStartsWith()
{
    ASSERT_TRUE( StringStartsWith( "", "", false ) );
    ASSERT_TRUE( StringStartsWith( "", "", true ) );
    ASSERT_TRUE( StringStartsWith( "strings start with themselves", "strings start with themselves", false ) );
    ASSERT_FALSE( StringStartsWith( "strings START with ThemselveS", "StRings start witH themselves", false ) );
    ASSERT_TRUE( StringStartsWith( "strings START with ThemselveS", "StRings start witH themselves", true ) );
    ASSERT_TRUE( StringStartsWith( "simple", "s", false ) );
    ASSERT_TRUE( StringStartsWith( "simple", "S", true ) );
    ASSERT_TRUE( StringStartsWith( "every string starts with empty string", "", true ) );
    ASSERT_FALSE( StringStartsWith( "", "empty string doesn't start with anything" ) );
    ASSERT_FALSE( StringStartsWith( "This String most certainly\r", "\nDoes not start with this 1." ) );
    ASSERT_TRUE( StringStartsWith( "abcdefg h i j k LMNOP", "abcdefg", false ) );
    ASSERT_FALSE( StringStartsWith( "abcdefg h i j k LMNOP", " abcdefg", false ) );
    ASSERT_FALSE( StringStartsWith( "  abcdefg h i j k LMNOP", "abcdefg", false ) );
    ASSERT_FALSE( StringStartsWith( "abcdefg h i j k LMNOP", "bcdefg", false ) );
    ASSERT_FALSE( StringStartsWith( "bcdefg h i j k LMNOP", "abcdefg", false ) );
    ASSERT_FALSE( StringStartsWith( "123abc908jfk HHH then some stuff", "123ABC908jFk HhH", false ) );
    ASSERT_TRUE( StringStartsWith( "123abc908jfk HHH then some stuff", "123ABC908jFk HhH", true ) );
    ASSERT_FALSE( StringStartsWith( "you can't start", "you can't start with something longer" ) );
}
static void TestStringEndsWith()
{
    ASSERT_TRUE( StringEndsWith( "", "", false ) );
    ASSERT_TRUE( StringEndsWith( "", "", true ) );
    ASSERT_TRUE( StringEndsWith( "strings end with themselves", "strings end with themselves", false ) );
    ASSERT_FALSE( StringEndsWith( "strings END with ThemselveS", "StRings end witH themselves", false ) );
    ASSERT_TRUE( StringEndsWith( "strings END with ThemselveS", "StRings end witH themselves", true ) );
    ASSERT_TRUE( StringEndsWith( "simple", "e", false ) );
    ASSERT_TRUE( StringEndsWith( "simple", "E", true ) );
    ASSERT_TRUE( StringEndsWith( "every string ends with empty string", "", true ) );
    ASSERT_FALSE( StringEndsWith( "", "empty string doesn't end with anything" ) );
    ASSERT_FALSE( StringEndsWith( "This STRING\r\t\v", "\nDoes not end w/ this 1." ) );
    ASSERT_TRUE( StringEndsWith( "abcdefg h i j k LMNOP", "k LMNOP", false ) );
    ASSERT_FALSE( StringEndsWith( "abcdefg h i j k LMNOP", "k LMNOP ", false ) );
    ASSERT_FALSE( StringEndsWith( "abcdefg h i j k LMNOP  ", "k LMNOP", false ) );
    ASSERT_FALSE( StringEndsWith( "abcdefg h i j k LMNOP", "j k LMNO", false ) );
    ASSERT_FALSE( StringEndsWith( "bcdefg h i j k LMNOP", "j k LMNOPQ", false ) );
    ASSERT_FALSE( StringEndsWith( "first some 123abc908jfk HHH", "123ABC908jFk HhH", false ) );
    ASSERT_TRUE( StringEndsWith( "first some 123abc908jfk HHH", "123ABC908jFk HhH", true ) );
    ASSERT_FALSE( StringEndsWith( "with something longer", "you can't end with something longer" ) );
}
static void TestStringContains()
{
    ASSERT_TRUE( StringContains( "", "", false ) );
    ASSERT_TRUE( StringContains( "", "", true ) );
    ASSERT_TRUE( StringContains( "contain themselves", "contain themselves", false ) );
    ASSERT_FALSE( StringContains( "ContaiN THEMselves", "conTAIn ThemSelveS", false ) );
    ASSERT_TRUE( StringContains( "ContaiN THEMselves", "conTAIn ThemSelveS", true ) );
    ASSERT_FALSE( StringContains( "", "contains nothing" ) );
    ASSERT_TRUE( StringContains( "everything contains", "" ) );
    ASSERT_TRUE( StringContains( "simple", "p", false ) );
    ASSERT_FALSE( StringContains( "simple", "P", false ) );
    ASSERT_TRUE( StringContains( "simple", "P", true ) );
    ASSERT_FALSE( StringContains( "a whole bunch of words", "but no this" ) );
    ASSERT_FALSE( StringContains( "This came close but no cigar", "cluse" ) );
    ASSERT_FALSE( StringContains( "This came close but no cigar", "closee" ) );
    ASSERT_FALSE( StringContains( "This came close but no cigar", "cclose" ) );
    ASSERT_TRUE( StringContains( "This came close but no cigar", "close" ) );
    ASSERT_TRUE( StringContains( "This came close but no cigar", " close" ) );
    ASSERT_TRUE( StringContains( "This came close but no cigar", "close " ) );
    ASSERT_TRUE( StringContains( "This came close but no cigar", " close " ) );
    ASSERT_FALSE( StringContains( "This CamE Close but no cigar", "caMe cLOSE", false ) );
    ASSERT_TRUE( StringContains( "This CamE Close but no cigar", "caMe cLOSE", true ) );
    ASSERT_TRUE( StringContains( "This CamE Close but no cigar", "this", true ) );
    ASSERT_TRUE( StringContains( "This CamE Close but no cigar", "cigar", true ) );
}
static void TestToLower()
{
    ASSERT_ARE_EQUAL( ToLower( "" ), "" );
    ASSERT_ARE_NOT_EQUAL( ToLower( "AnyThiNG" ),  "" );
    ASSERT_ARE_EQUAL( ToLower( "AnyThiNG" ), "anything" );
    ASSERT_ARE_NOT_EQUAL( ToLower( "2 3:/sd#*%& @9a4B8C7" ), "2 3:/sd#*%& @9a4B8C7" );
    ASSERT_ARE_EQUAL( ToLower( "2 3:/sd#*%& @9a4B8C7" ), "2 3:/sd#*%& @9a4b8c7" );
    ASSERT_ARE_EQUAL( ToLower( "\n\vA\r\t Bc" ), "\n\va\r\t bc" );
}
static void TestToUpper()
{
    ASSERT_ARE_EQUAL( ToUpper( "" ), "" );
    ASSERT_ARE_NOT_EQUAL( ToUpper( "AnyThiNG" ), "" );
    ASSERT_ARE_EQUAL( ToUpper( "AnyThiNG" ), "ANYTHING" );
    ASSERT_ARE_NOT_EQUAL( ToUpper( "2 3:/sd#*%& @9a4B8C7" ), "2 3:/sd#*%& @9a4B8C7" );
    ASSERT_ARE_EQUAL( ToUpper( "2 3:/sd#*%& @9a4B8C7" ), "2 3:/SD#*%& @9A4B8C7" );
    ASSERT_ARE_EQUAL( ToUpper( "\n\vA\r\t Bc" ), "\n\vA\r\t BC" );
}
static void TestReplaceAll()
{
    ASSERT_ARE_EQUAL( ReplaceAll( "", "", "", false ), "" );
    ASSERT_ARE_EQUAL( ReplaceAll( "", "", "", true ), "" );
    ASSERT_ARE_EQUAL( ReplaceAll( "Some text goes here", "", "", false ), "Some text goes here" );
    ASSERT_ARE_EQUAL( ReplaceAll( "Some text goes here", "", "", true ), "Some text goes here" );
    ASSERT_ARE_EQUAL( ReplaceAll( "Some text goes here", "", "by definition" ), "Some text goes here" );
    ASSERT_ARE_EQUAL( ReplaceAll( "", "", "by definition" ), "by definition" );
    ASSERT_ARE_EQUAL( ReplaceAll( "Some text goes here", "GoEs", "not", false ), "Some text goes here" );
    ASSERT_ARE_EQUAL( ReplaceAll( "Some text goes here", "GoEs", "not", true ), "Some text not here" );
    ASSERT_ARE_EQUAL( ReplaceAll( "Some text goes here", "SOME teXT GoES", "", true ), " here" );
    ASSERT_ARE_EQUAL( ReplaceAll( "Some text goes here", "here", "", true ), "Some text goes " );
    ASSERT_ARE_EQUAL( ReplaceAll( "Multi replace multi replaced mulTI, replacement mult", "multi", " a ", true ),
                      " a  replace  a  replaced  a , replacement mult" );
    ASSERT_ARE_EQUAL( ReplaceAll( "petrsnd:x:10:23:blah:/home/petrsnd:/bin/bash", ":", "", true ),
                      "petrsndx1023blah/home/petrsnd/bin/bash" );
}
static void TestTrimRight()
{
    ASSERT_ARE_EQUAL( TrimRight( "" ), "" );
    ASSERT_ARE_EQUAL( TrimRight( " \r\n\t   \t\t\n\n\n   " ), "" );
    ASSERT_ARE_EQUAL( TrimRight( "\t \n\n\r\r \t\t    blah" ), "\t \n\n\r\r \t\t    blah" );
    ASSERT_ARE_EQUAL( TrimRight( "blah \t\t \n\n\r\r \t\t    " ), "blah" );
    ASSERT_ARE_EQUAL( TrimRight( "  \t\n\rblah \t\t \n\n\r\r \t\t    " ), "  \t\n\rblah" );
    ASSERT_ARE_EQUAL( TrimRight( "  \t\n\rblah \t\t blah\n\n\r\r \t\t    " ), "  \t\n\rblah \t\t blah" );
}
static void TestTrimLeft()
{
    ASSERT_ARE_EQUAL( TrimLeft( "" ), "" );
    ASSERT_ARE_EQUAL( TrimLeft( " \r\n\t   \t\t\n\n\n   " ), "" );
    ASSERT_ARE_EQUAL( TrimLeft( "\t \n\n\r\r \t\t    blah" ), "blah" );
    ASSERT_ARE_EQUAL( TrimLeft( "blah \t\t \n\n\r\r \t\t    " ), "blah \t\t \n\n\r\r \t\t    " );
    ASSERT_ARE_EQUAL( TrimLeft( "  \t\n\rblah \t\t \n\n\r\r \t\t    " ), "blah \t\t \n\n\r\r \t\t    " );
    ASSERT_ARE_EQUAL( TrimLeft( "  \t\n\rblah \t\t blah\n\n\r\r \t\t    " ), "blah \t\t blah\n\n\r\r \t\t    " );
}
static void TestTrim()
{
    ASSERT_ARE_EQUAL( Trim( "" ), "" );
    ASSERT_ARE_EQUAL( Trim( " \r\n\t   \t\t\n\n\n   " ), "" );
    ASSERT_ARE_EQUAL( Trim( "\t \n\n\r\r \t\t    blah" ), "blah" );
    ASSERT_ARE_EQUAL( Trim( "blah \t\t \n\n\r\r \t\t    " ), "blah" );
    ASSERT_ARE_EQUAL( Trim( "  \t\n\rblah \t\t \n\n\r\r \t\t    " ), "blah" );
    ASSERT_ARE_EQUAL( Trim( "  \t\n\rblah \t\t blah\n\n\r\r \t\t    " ), "blah \t\t blah" );
}
static void TestStringIsNumeric()
{
    ASSERT_FALSE( StringIsNumeric( "" ) );
    ASSERT_FALSE( StringIsNumeric( "a" ) );
    ASSERT_FALSE( StringIsNumeric( "abcd ef g" ) );
    ASSERT_FALSE( StringIsNumeric( "123a893ajfj anoth" ) );
    ASSERT_FALSE( StringIsNumeric( "a123" ) );
    ASSERT_FALSE( StringIsNumeric( "123a" ) );
    ASSERT_FALSE( StringIsNumeric( "1a2" ) );
    ASSERT_TRUE( StringIsNumeric( "1" ) );
    ASSERT_TRUE( StringIsNumeric( "123 " ) );
    ASSERT_TRUE( StringIsNumeric( "123 " ) );
    ASSERT_TRUE( StringIsNumeric( " 123 " ) );
    ASSERT_TRUE( StringIsNumeric( "98092830" ) );
    ASSERT_TRUE( StringIsNumeric( "000000000098092830" ) );
    ASSERT_TRUE( StringIsNumeric( "98092830" ) );
    ASSERT_TRUE( StringIsNumeric( "-98092830" ) );
    ASSERT_TRUE( StringIsNumeric( " -98092830  " ) );
    ASSERT_FALSE( StringIsNumeric( " -9809 2830  " ) );
    ASSERT_TRUE( StringIsNumeric( "  -29203849.39383930  \t" ) );
    ASSERT_FALSE( StringIsNumeric( "  -29203849.393a83930 \n \t" ) );
    ASSERT_FALSE( StringIsNumeric( "  -29203849.3938.3930  \t" ) );
}

int main( int argc, char** argv )
{
    bool stringUtilsAllPassed;
    DECLARE_UNIT_TEST_SUITE( StringUtils )
        ADD_UNIT_TEST( StringUtils, TestStringsCompare )
        ADD_UNIT_TEST( StringUtils, TestStringsAreEqual )
        ADD_UNIT_TEST( StringUtils, TestStringStartsWith )
        ADD_UNIT_TEST( StringUtils, TestStringEndsWith )
        ADD_UNIT_TEST( StringUtils, TestStringContains )
        ADD_UNIT_TEST( StringUtils, TestToLower )
        ADD_UNIT_TEST( StringUtils, TestToUpper )
        ADD_UNIT_TEST( StringUtils, TestReplaceAll )
        ADD_UNIT_TEST( StringUtils, TestTrimRight )
        ADD_UNIT_TEST( StringUtils, TestTrimLeft )
        ADD_UNIT_TEST( StringUtils, TestTrim )
        ADD_UNIT_TEST( StringUtils, TestStringIsNumeric )
    END_UNIT_TEST_SUITE( StringUtils, stringUtilsAllPassed )

    return ( stringUtilsAllPassed ? 0 : 1 );
}
