// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "DraftMiller.h"

#include "UnitTestSuite.h"

using namespace Magenta;

// Anonymous namespace for registrations
namespace {

bool reg = DraftMiller::Instance()->Register( SSH_AGENTC_REQUEST_IDENTITIES,
                                              []( const DmMessage::Ptr& message ) -> DmMessage::Ptr
    {
        std::cout << "made it!" << std::endl;
        // TODO:
        return nullptr;
    } );

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
}

int main( int argc, char** argv )
{
    bool testsAllPassed;
    DECLARE_UNIT_TEST_SUITE( RequestIdentities )
        ADD_UNIT_TEST( RequestIdentities, TestRequestResponse );
    END_UNIT_TEST_SUITE( RequestIdentities, testsAllPassed )

    return ( testsAllPassed ? 0 : 1 );
}