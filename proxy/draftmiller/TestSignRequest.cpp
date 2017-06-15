// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "DraftMiller.h"

#include "UnitTestSuite.h"

using namespace Magenta;

// Anonymous namespace for registrations
namespace {

bool reg = DraftMiller::Instance()->Register( SSH_AGENTC_SIGN_REQUEST,
                                              []( const DmMessage::Ptr& message ) -> DmMessage::Ptr
    {
        ASSERT_ARE_EQUAL( message->Number, SSH_AGENTC_SIGN_REQUEST );
        auto signRequest = std::dynamic_pointer_cast< DmSignRequest >( message );
        ASSERT_ARE_EQUAL( signRequest->KeyBlob, Buffer( { 0xba, 0xbe } ) );
        ASSERT_ARE_EQUAL( signRequest->Data, Buffer( { 0xfe, 0xeb, 0x1e } ) );
        ASSERT_ARE_EQUAL( signRequest->Flags, 3434 );
        DmSignResponse::Ptr signResponse = std::make_shared< DmSignResponse >();
        signResponse->Signature = { 0x00, 0x01, 0x02, 0x03, 0x04, 0xa0, 0xaf, 0xde, 0xad, 0xbe, 0xef };
        return signResponse;
    } );

}

// Real data
Buffer s_realSignRequest = {
    0x00, 0x00, 0x02, 0x83, 0x0d, 0x00, 0x00, 0x01, 0x15, 0x00, 0x00, 0x00,
    0x07, 0x73, 0x73, 0x68, 0x2d, 0x72, 0x73, 0x61, 0x00, 0x00, 0x00, 0x01,
    0x23, 0x00, 0x00, 0x01, 0x01, 0x00, 0xb1, 0x36, 0x39, 0x01, 0xca, 0x88,
    0x2d, 0xfa, 0xf9, 0xa5, 0x67, 0x21, 0x5b, 0x11, 0x52, 0x99, 0x38, 0x7a,
    0xa3, 0xe7, 0x74, 0xaf, 0xa9, 0x7f, 0x9f, 0x0d, 0xc0, 0x3b, 0x03, 0x2c,
    0xe7, 0xec, 0x8a, 0x04, 0xa1, 0x72, 0xbb, 0xf8, 0x31, 0x5f, 0xcf, 0x0f,
    0x3a, 0x9d, 0x77, 0xe9, 0xf5, 0x14, 0x70, 0xe0, 0xf2, 0x12, 0x04, 0x5f,
    0x20, 0x09, 0xf2, 0x69, 0xb8, 0x4c, 0xb0, 0x30, 0xf0, 0x31, 0x05, 0x17,
    0x34, 0xce, 0x8a, 0x1f, 0x57, 0xa8, 0x83, 0x2c, 0xa7, 0x7f, 0xdd, 0xe1,
    0x40, 0x20, 0x4a, 0x35, 0x56, 0x54, 0x79, 0xad, 0x07, 0xaa, 0xf2, 0x46,
    0xe4, 0x08, 0x0e, 0xd9, 0x5d, 0xa0, 0xb4, 0xae, 0xb0, 0x94, 0xb1, 0x2c,
    0x67, 0xef, 0x40, 0x00, 0x14, 0x42, 0xd9, 0xfc, 0x35, 0xe4, 0x6d, 0x9f,
    0x0a, 0x4f, 0x3d, 0xd1, 0x42, 0xf6, 0xf0, 0xf8, 0xe8, 0x74, 0xff, 0x99,
    0x91, 0xd4, 0xb2, 0x6d, 0x14, 0x71, 0x99, 0x82, 0x09, 0x2a, 0xe3, 0xba,
    0x98, 0x9e, 0x17, 0xbc, 0x13, 0x99, 0x98, 0x67, 0x47, 0x44, 0xb6, 0x8c,
    0xb3, 0x01, 0x4c, 0x6e, 0x67, 0x0e, 0x91, 0x97, 0x14, 0xfb, 0xcf, 0x39,
    0x6d, 0x9d, 0xd6, 0xc3, 0xb3, 0xe4, 0x58, 0xf7, 0x1e, 0x4a, 0xec, 0xe0,
    0xde, 0xb0, 0x54, 0x0a, 0x52, 0x11, 0x4e, 0xe1, 0xa1, 0x74, 0x8a, 0xe3,
    0xf8, 0xbb, 0x5c, 0x8e, 0x00, 0x4f, 0x67, 0x64, 0xa3, 0x4a, 0xee, 0x0a,
    0x73, 0xfa, 0x4b, 0xbb, 0xed, 0x29, 0x4a, 0xdd, 0x41, 0x33, 0xa2, 0xac,
    0x47, 0x8e, 0xc6, 0x10, 0xfd, 0xff, 0x14, 0x7d, 0x87, 0x3f, 0xde, 0x42,
    0x50, 0xdf, 0x7d, 0x5a, 0x59, 0x3b, 0x02, 0x03, 0xf1, 0xf9, 0x2d, 0xfd,
    0x26, 0xc3, 0x16, 0x12, 0x37, 0xdf, 0x7c, 0x20, 0xbe, 0x04, 0xaf, 0xd4,
    0x17, 0x58, 0xc8, 0x36, 0x16, 0xad, 0x7f, 0x2f, 0xbc, 0x33, 0x00, 0x00,
    0x01, 0x65, 0x00, 0x00, 0x00, 0x14, 0xc3, 0xcd, 0x83, 0xd7, 0xfb, 0x0d,
    0x84, 0xaf, 0x8d, 0xa1, 0x4f, 0x1e, 0xdd, 0x93, 0xe7, 0x08, 0xd4, 0x9d,
    0xf0, 0x5c, 0x32, 0x00, 0x00, 0x00, 0x04, 0x6d, 0x61, 0x74, 0x74, 0x00,
    0x00, 0x00, 0x0e, 0x73, 0x73, 0x68, 0x2d, 0x63, 0x6f, 0x6e, 0x6e, 0x65,
    0x63, 0x74, 0x69, 0x6f, 0x6e, 0x00, 0x00, 0x00, 0x09, 0x70, 0x75, 0x62,
    0x6c, 0x69, 0x63, 0x6b, 0x65, 0x79, 0x01, 0x00, 0x00, 0x00, 0x07, 0x73,
    0x73, 0x68, 0x2d, 0x72, 0x73, 0x61, 0x00, 0x00, 0x01, 0x15, 0x00, 0x00,
    0x00, 0x07, 0x73, 0x73, 0x68, 0x2d, 0x72, 0x73, 0x61, 0x00, 0x00, 0x00,
    0x01, 0x23, 0x00, 0x00, 0x01, 0x01, 0x00, 0xb1, 0x36, 0x39, 0x01, 0xca,
    0x88, 0x2d, 0xfa, 0xf9, 0xa5, 0x67, 0x21, 0x5b, 0x11, 0x52, 0x99, 0x38,
    0x7a, 0xa3, 0xe7, 0x74, 0xaf, 0xa9, 0x7f, 0x9f, 0x0d, 0xc0, 0x3b, 0x03,
    0x2c, 0xe7, 0xec, 0x8a, 0x04, 0xa1, 0x72, 0xbb, 0xf8, 0x31, 0x5f, 0xcf,
    0x0f, 0x3a, 0x9d, 0x77, 0xe9, 0xf5, 0x14, 0x70, 0xe0, 0xf2, 0x12, 0x04,
    0x5f, 0x20, 0x09, 0xf2, 0x69, 0xb8, 0x4c, 0xb0, 0x30, 0xf0, 0x31, 0x05,
    0x17, 0x34, 0xce, 0x8a, 0x1f, 0x57, 0xa8, 0x83, 0x2c, 0xa7, 0x7f, 0xdd,
    0xe1, 0x40, 0x20, 0x4a, 0x35, 0x56, 0x54, 0x79, 0xad, 0x07, 0xaa, 0xf2,
    0x46, 0xe4, 0x08, 0x0e, 0xd9, 0x5d, 0xa0, 0xb4, 0xae, 0xb0, 0x94, 0xb1,
    0x2c, 0x67, 0xef, 0x40, 0x00, 0x14, 0x42, 0xd9, 0xfc, 0x35, 0xe4, 0x6d,
    0x9f, 0x0a, 0x4f, 0x3d, 0xd1, 0x42, 0xf6, 0xf0, 0xf8, 0xe8, 0x74, 0xff,
    0x99, 0x91, 0xd4, 0xb2, 0x6d, 0x14, 0x71, 0x99, 0x82, 0x09, 0x2a, 0xe3,
    0xba, 0x98, 0x9e, 0x17, 0xbc, 0x13, 0x99, 0x98, 0x67, 0x47, 0x44, 0xb6,
    0x8c, 0xb3, 0x01, 0x4c, 0x6e, 0x67, 0x0e, 0x91, 0x97, 0x14, 0xfb, 0xcf,
    0x39, 0x6d, 0x9d, 0xd6, 0xc3, 0xb3, 0xe4, 0x58, 0xf7, 0x1e, 0x4a, 0xec,
    0xe0, 0xde, 0xb0, 0x54, 0x0a, 0x52, 0x11, 0x4e, 0xe1, 0xa1, 0x74, 0x8a,
    0xe3, 0xf8, 0xbb, 0x5c, 0x8e, 0x00, 0x4f, 0x67, 0x64, 0xa3, 0x4a, 0xee,
    0x0a, 0x73, 0xfa, 0x4b, 0xbb, 0xed, 0x29, 0x4a, 0xdd, 0x41, 0x33, 0xa2,
    0xac, 0x47, 0x8e, 0xc6, 0x10, 0xfd, 0xff, 0x14, 0x7d, 0x87, 0x3f, 0xde,
    0x42, 0x50, 0xdf, 0x7d, 0x5a, 0x59, 0x3b, 0x02, 0x03, 0xf1, 0xf9, 0x2d,
    0xfd, 0x26, 0xc3, 0x16, 0x12, 0x37, 0xdf, 0x7c, 0x20, 0xbe, 0x04, 0xaf,
    0xd4, 0x17, 0x58, 0xc8, 0x36, 0x16, 0xad, 0x7f, 0x2f, 0xbc, 0x33
};

Buffer s_realSignResponse = {
    0x00, 0x00, 0x01, 0x14, 0x0e, 0x00, 0x00, 0x01, 0x0f, 0x00, 0x00, 0x00,
    0x07, 0x73, 0x73, 0x68, 0x2d, 0x72, 0x73, 0x61, 0x00, 0x00, 0x01, 0x00,
    0x6d, 0x35, 0xb5, 0xd4, 0xe0, 0x54, 0xe5, 0xc9, 0x20, 0x61, 0xfa, 0xcb,
    0xe1, 0xdf, 0xf8, 0xa6, 0xf9, 0x49, 0x45, 0xaa, 0x9e, 0xc5, 0xe4, 0x88,
    0x4e, 0x07, 0xad, 0x48, 0xac, 0xd8, 0x7a, 0x09, 0x33, 0x2c, 0x73, 0x8a,
    0xf7, 0x43, 0x86, 0x0e, 0x35, 0x94, 0x68, 0xd9, 0x1f, 0xd7, 0x4f, 0x8f,
    0xef, 0x58, 0xfc, 0x4b, 0x52, 0x4b, 0xa1, 0x57, 0x36, 0xd8, 0xee, 0x15,
    0xb3, 0x2e, 0xd2, 0x64, 0x78, 0x68, 0xb0, 0xb5, 0x4c, 0xfc, 0x86, 0x67,
    0x31, 0x52, 0xdc, 0x9c, 0x94, 0x9c, 0xb0, 0x6c, 0xf5, 0x7e, 0x0a, 0xd1,
    0x80, 0x44, 0x01, 0x31, 0x75, 0x04, 0x7f, 0xa9, 0x28, 0xc8, 0xdb, 0xac,
    0x3b, 0xeb, 0xd7, 0x35, 0x7e, 0x66, 0x12, 0xb6, 0x86, 0xb4, 0xef, 0xa4,
    0x03, 0x30, 0xc3, 0xc8, 0xfb, 0x0d, 0x25, 0x5a, 0xa1, 0x98, 0x26, 0x04,
    0x1d, 0x29, 0xfc, 0x84, 0x04, 0xee, 0x44, 0xe7, 0xb8, 0x80, 0xb6, 0xe6,
    0x9b, 0xca, 0xa4, 0xc7, 0x03, 0xd9, 0x82, 0xd1, 0x30, 0x2c, 0x6f, 0x78,
    0x2f, 0x5b, 0x07, 0x4d, 0xf6, 0x8d, 0x26, 0x1d, 0x07, 0x78, 0x6a, 0x4a,
    0x18, 0x3e, 0x7e, 0x57, 0x1a, 0x4b, 0x37, 0x88, 0x83, 0xd6, 0x9d, 0xe8,
    0x0c, 0xa2, 0x53, 0xc4, 0x0c, 0xc5, 0xd9, 0x85, 0x4c, 0x84, 0x19, 0xf3,
    0xb7, 0xaf, 0x6a, 0x10, 0xa6, 0xf8, 0x17, 0x86, 0xaf, 0x20, 0xa0, 0x15,
    0xd7, 0x89, 0xf2, 0x51, 0x4d, 0x36, 0xb8, 0x23, 0x2b, 0xde, 0x65, 0xda,
    0xc2, 0x9f, 0x62, 0xce, 0x57, 0x47, 0xb1, 0x16, 0xfb, 0xfb, 0xff, 0x48,
    0x8f, 0x80, 0xf6, 0x95, 0x83, 0x39, 0x32, 0xd2, 0x7f, 0x68, 0xa8, 0x90,
    0xa7, 0x5f, 0x42, 0xcf, 0x91, 0x2f, 0x97, 0x90, 0xcf, 0xb3, 0x5a, 0x68,
    0xd7, 0xe0, 0xce, 0xa7, 0xd2, 0x31, 0xfd, 0x5e, 0x05, 0x22, 0xe0, 0xe8,
    0xb2, 0x6b, 0xc2, 0xf8
};

static void TestEncodeParseRequest()
{
    auto request = std::make_shared< DmSignRequest >();
    request->KeyBlob = { 0x12, 0x34, 0x45, 0x67, 0x89 };
    request->Data = { 0xab, 0xcd, 0xef };
    request->Flags = 123456789;
    Buffer requestEncoded = DmEncodeMessage( request );
    auto requestParsed = std::dynamic_pointer_cast< DmSignRequest >( DmParseMessage( requestEncoded ) );
    ASSERT_FALSE( requestParsed == nullptr );
    ASSERT_ARE_EQUAL( request->Number, requestParsed->Number );
    ASSERT_ARE_EQUAL( request->KeyBlob, requestParsed->KeyBlob );
    ASSERT_ARE_EQUAL( request->Data, requestParsed->Data );
    ASSERT_ARE_EQUAL( request->Flags, requestParsed->Flags );
}

static void TestEncodeParseResponse()
{
    auto response = std::make_shared< DmSignResponse >();
    response->Signature = { 0x20, 0x21, 0x22, 0x23 };
    Buffer responseEncoded = DmEncodeMessage( response );
    auto responseParsed = DmParseMessage( responseEncoded );
    ASSERT_FALSE( std::dynamic_pointer_cast< DmSignResponse >( responseParsed ) == nullptr );
    ASSERT_ARE_EQUAL( response->Number, responseParsed->Number );
    ASSERT_ARE_EQUAL( response->Signature, Buffer( { 0x20, 0x21, 0x22, 0x23 } ) );
}

static void TestRequestResponse()
{
    auto request = std::make_shared< DmSignRequest >();
    request->KeyBlob = { 0xba, 0xbe };
    request->Data = { 0xfe, 0xeb, 0x1e };
    request->Flags = 3434;
    Buffer requestBuffer = DmEncodeMessage( std::dynamic_pointer_cast< DmMessage >( request ) );
    Buffer responseBuffer = DraftMiller::Instance()->HandleMessage( requestBuffer );
    auto message = DmParseMessage( responseBuffer );
    if ( message->Number == SSH_AGENT_FAILURE )
        throw UnitTestException( "DraftMiller return SSH_AGENT_FAILURE" );
    auto response = std::dynamic_pointer_cast< DmSignResponse >( message );
    ASSERT_FALSE( response == nullptr );
    ASSERT_ARE_EQUAL( response->Number, SSH_AGENT_SIGN_RESPONSE );
    ASSERT_ARE_EQUAL( response->Signature,
                      Buffer( { 0x00, 0x01, 0x02, 0x03, 0x04, 0xa0, 0xaf, 0xde, 0xad, 0xbe, 0xef } ) );
}

static void TestRealData()
{
    Buffer expectedSignRequest( s_realSignRequest );
    Buffer expectedSignResponse( s_realSignResponse );
    auto signRequest = DmParseMessage( s_realSignRequest );
    auto signResponse = DmParseMessage( s_realSignResponse );
    ASSERT_FALSE( signRequest == nullptr );
    ASSERT_FALSE( signResponse == nullptr );
    Buffer signRequestEncoded = DmEncodeMessage( signRequest );
    Buffer signResponseEncoded = DmEncodeMessage( signResponse );
    ASSERT_ARE_EQUAL( expectedSignRequest, signRequestEncoded );
    ASSERT_ARE_EQUAL( expectedSignResponse, signResponseEncoded );
}

int main( int argc, char** argv )
{
    bool testsAllPassed;
    DECLARE_UNIT_TEST_SUITE( SignRequest )
        ADD_UNIT_TEST( SignRequest, TestEncodeParseRequest );
        ADD_UNIT_TEST( SignRequest, TestEncodeParseResponse );
        ADD_UNIT_TEST( SignRequest, TestRequestResponse );
        ADD_UNIT_TEST( SignRequest, TestRealData );
    END_UNIT_TEST_SUITE( SignRequest, testsAllPassed )

    return ( testsAllPassed ? 0 : 1 );
}
