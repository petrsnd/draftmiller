// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "DraftMiller.h"

#include "UnitTestSuite.h"

using namespace Magenta;

// Anonymous namespace for registrations
namespace {

bool reg = DraftMiller::Instance()->Register( SSH_AGENTC_REQUEST_IDENTITIES,
                                              []( const DmMessage::Ptr& message ) -> DmMessage::Ptr
    {
        ASSERT_ARE_EQUAL( message->Number, SSH_AGENTC_REQUEST_IDENTITIES );
        // TODO: read response from file
        DmIdentitiesAnswer::Ptr identitiesAnswer = std::make_shared< DmIdentitiesAnswer >();
        identitiesAnswer->NumberKeys = 2;
        identitiesAnswer->Keys.push_back( { { 0x00, 0x01, 0x02, 0x03 }, "abcdef" } );
        identitiesAnswer->Keys.push_back( { { 0x10, 0x11, 0x12, 0x13 }, "uvwxyz" } );
        return identitiesAnswer;
    } );

}

static void TestEncodeParseRequest()
{
    auto request = std::make_shared< DmRequestIdentities >();
    Buffer requestEncoded = DmEncodeMessage( request );
    auto requestParsed = DmParseMessage( requestEncoded );
    ASSERT_FALSE( std::dynamic_pointer_cast< DmRequestIdentities >( requestParsed ) == nullptr );
    ASSERT_ARE_EQUAL( request->Number, requestParsed->Number );
}

static void TestEncodeParseResponse()
{
    auto response = std::make_shared< DmIdentitiesAnswer >();
    response->NumberKeys = 3;
    response->Keys.push_back( { { 0x20, 0x21, 0x22, 0x23 }, "hello world!" } );
    response->Keys.push_back( { { 0x50, 0x51, 0x52, 0x53 }, "foobar" } );
    response->Keys.push_back( { { 0x50, 0x51, 0x52, 0x53, 0x60, 0x61, 0x62 }, "Magenta " } );
    Buffer responseEncoded = DmEncodeMessage( response );
    auto responseParsed = DmParseMessage( responseEncoded );
    ASSERT_FALSE( std::dynamic_pointer_cast< DmIdentitiesAnswer >( responseParsed ) == nullptr );
    ASSERT_ARE_EQUAL( response->Number, responseParsed->Number );
    ASSERT_ARE_EQUAL( response->NumberKeys, 3 );
    ASSERT_ARE_EQUAL( response->Keys[0].KeyBlob, Buffer( { 0x20, 0x21, 0x22, 0x23 } ) );
    ASSERT_ARE_EQUAL( response->Keys[0].Comment, "hello world!" );
    ASSERT_ARE_EQUAL( response->Keys[1].KeyBlob, Buffer( { 0x50, 0x51, 0x52, 0x53 } ) );
    ASSERT_ARE_EQUAL( response->Keys[1].Comment, "foobar" );
    ASSERT_ARE_EQUAL( response->Keys[2].KeyBlob, Buffer( { 0x50, 0x51, 0x52, 0x53, 0x60, 0x61, 0x62 } ) );
    ASSERT_ARE_EQUAL( response->Keys[2].Comment, "Magenta " );
}

static void TestRequestResponse()
{
    // TODO: read this buffer from a file
    auto request = std::make_shared< DmRequestIdentities >();
    Buffer requestBuffer = DmEncodeMessage( std::dynamic_pointer_cast< DmMessage >( request ) );
    // TODO: validate request buffer contents
    Buffer responseBuffer = DraftMiller::Instance()->HandleMessage( requestBuffer );
    auto message = DmParseMessage( responseBuffer );
    if ( message->Number == SSH_AGENT_FAILURE )
        throw UnitTestException( "DraftMiller return SSH_AGENT_FAILURE" );
    auto response = std::dynamic_pointer_cast< DmIdentitiesAnswer >( message );
    // TODO: validate response buffer contents against a file
    ASSERT_FALSE( response == nullptr );
    ASSERT_ARE_EQUAL( response->Number, SSH_AGENT_IDENTITIES_ANSWER );
    ASSERT_ARE_EQUAL( response->NumberKeys, 2 );
    ASSERT_ARE_EQUAL( response->Keys[0].KeyBlob, Buffer( { 0x00, 0x01, 0x02, 0x03 } ) );
    ASSERT_ARE_EQUAL( response->Keys[0].Comment, "abcdef" );
    ASSERT_ARE_EQUAL( response->Keys[1].KeyBlob, Buffer( { 0x10, 0x11, 0x12, 0x13 } ) );
    ASSERT_ARE_EQUAL( response->Keys[1].Comment, "uvwxyz" );
}

int main( int argc, char** argv )
{
    bool testsAllPassed;
    DECLARE_UNIT_TEST_SUITE( RequestIdentities )
        ADD_UNIT_TEST( RequestIdentities, TestEncodeParseRequest );
        ADD_UNIT_TEST( RequestIdentities, TestEncodeParseResponse );
        ADD_UNIT_TEST( RequestIdentities, TestRequestResponse );
    END_UNIT_TEST_SUITE( RequestIdentities, testsAllPassed )

    return ( testsAllPassed ? 0 : 1 );
}