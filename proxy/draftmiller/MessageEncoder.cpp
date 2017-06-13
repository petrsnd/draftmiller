// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "DraftMiller.h"

#include <DataBuffer.h>
#include <StringUtils.h>

namespace Magenta {

// Encode primitives
static void DmEncodeString( DataBuffer& db, const std::string& str )
{
    db.WriteUInt32( static_cast< uint32_t >( str.length() ) );
    db.WriteString( str, false );
}

static void DmEncodeString( DataBuffer& db, const Buffer& buf )
{
    db.WriteUInt32( static_cast< uint32_t >( buf.size() ) );
    db.WriteBuffer( buf );
}

static Buffer DmEncodePacket( const DataBuffer& db )
{
    DataBuffer packet;
    packet.WriteUInt32( static_cast< uint32_t >( db.Size() ) );
    packet.WriteBuffer( db );
    return packet;
}

// Encode responses
static Buffer DmEncodeFailure()
{
    DataBuffer db;
    db.WriteUInt8( SSH_AGENT_FAILURE );
    return DmEncodePacket( db );
}

static Buffer DmEncodeSuccess()
{
    DataBuffer db;
    db.WriteUInt8( SSH_AGENT_SUCCESS );
    return DmEncodePacket( db );
}

static Buffer DmEncodeIdentitiesAnswer( const DmIdentitiesAnswer::Ptr& identitiesAnswer )
{
    if ( identitiesAnswer == nullptr )
    {
        throw DmEncodeException( "Identities answer structure cannot be null" );
    }
    if ( identitiesAnswer->NumberKeys != identitiesAnswer->Keys.size() )
    {
        throw DmEncodeException(
            SC() << "Malformed identities structure, NumberKeys (" << identitiesAnswer->NumberKeys
                 << ") does not equal the size of Keys (" << identitiesAnswer->Keys.size() << ")" );
    }
    DataBuffer db;
    db.WriteUInt8( identitiesAnswer->Number );
    db.WriteUInt32( identitiesAnswer->NumberKeys );
    for ( auto i = 0; i < identitiesAnswer->NumberKeys; i++ )
    {
        DmEncodeString( db, identitiesAnswer->Keys[i].KeyBlob );
        DmEncodeString( db, identitiesAnswer->Keys[i].Comment );
    }
    return DmEncodePacket( db );
}

static Buffer DmEncodeSignResponse( const DmSignResponse::Ptr& signResponse )
{
    if ( signResponse == nullptr )
    {
        throw DmEncodeException( "Sign response structure cannot be null" );
    }
    DataBuffer db;
    db.WriteUInt8( signResponse->Number );
    DmEncodeString( db, signResponse->Signature );
    return DmEncodePacket( db );
}

// Encode requests
static Buffer DmEncodeRequestIdentities( const DmRequestIdentities::Ptr& requestIdentities )
{
    if ( requestIdentities == nullptr )
    {
        throw DmEncodeException( "Request identities structure cannot be null" );
    }
    DataBuffer db;
    db.WriteUInt8( requestIdentities->Number );
    return DmEncodePacket( db );
}

static Buffer DmEncodeSignRequest( const DmSignRequest::Ptr& signRequest )
{
    if ( signRequest == nullptr )
    {
        throw DmEncodeException( "Sign request structure cannot be null" );
    }
    DataBuffer db;
    db.WriteUInt8( signRequest->Number );
    db.WriteUInt32( static_cast< uint32_t >( signRequest->KeyBlob.size() ) );
    db.WriteBuffer( signRequest->KeyBlob );
    db.WriteUInt32( static_cast< uint32_t >( signRequest->Data.size() ) );
    db.WriteBuffer( signRequest->Data );
    db.WriteUInt32( signRequest->Flags );
    return DmEncodePacket( db );
}

Buffer DmEncodeMessage( const DmMessage::Ptr& message )
{
    switch ( message->Number )
    {
        case SSH_LEGACY_RESERVED_MESSAGE_1:
        case SSH_LEGACY_RESERVED_MESSAGE_2:
        case SSH_LEGACY_RESERVED_MESSAGE_3:
        case SSH_LEGACY_RESERVED_MESSAGE_4:
        case SSH_LEGACY_RESERVED_MESSAGE_7:
        case SSH_LEGACY_RESERVED_MESSAGE_8:
        case SSH_LEGACY_RESERVED_MESSAGE_9:
        case SSH_LEGACY_RESERVED_MESSAGE_10:
        case SSH_LEGACY_RESERVED_MESSAGE_15:
        case SSH_LEGACY_RESERVED_MESSAGE_16:
        case SSH_LEGACY_RESERVED_MESSAGE_24:
            throw DmEncodeException( SC() << "Legacy message not supported (" << message->Number << ")" );

        // Responses
        case SSH_AGENT_FAILURE:
            return DmEncodeFailure();
        case SSH_AGENT_SUCCESS:
            return DmEncodeSuccess();
        case SSH_AGENT_IDENTITIES_ANSWER:
            return DmEncodeIdentitiesAnswer( std::dynamic_pointer_cast< DmIdentitiesAnswer >( message ) );
        case SSH_AGENT_SIGN_RESPONSE:
            return DmEncodeSignResponse( std::dynamic_pointer_cast< DmSignResponse >( message ) );
        case SSH_AGENT_EXTENSION_FAILURE:
            throw DmEncodeException(
                SC() << "Response message not currently supported for encoding (" << message->Number << ")" );

        // Requests
        case SSH_AGENTC_REQUEST_IDENTITIES:
            return DmEncodeRequestIdentities( std::dynamic_pointer_cast< DmRequestIdentities >( message ) );
        case SSH_AGENTC_SIGN_REQUEST:
            return DmEncodeSignRequest( std::dynamic_pointer_cast< DmSignRequest >( message ) );

        case SSH_AGENTC_ADD_IDENTITY:
        case SSH_AGENTC_REMOVE_IDENTITY:
            throw DmEncodeException(
                SC() << "Request message not currently supported for encoding (" << message->Number << ")" );
        case SSH_AGENTC_REMOVE_ALL_IDENTITIES:

        case SSH_AGENTC_ADD_SMARTCARD_KEY:
        case SSH_AGENTC_REMOVE_SMARTCARD_KEY:
        case SSH_AGENTC_LOCK:
        case SSH_AGENTC_UNLOCK:
        case SSH_AGENTC_ADD_ID_CONSTRAINED:
        case SSH_AGENTC_ADD_SMARTCARD_KEY_CONSTRAINED:
        case SSH_AGENTC_EXTENSION:
            throw DmEncodeException(
                SC() << "Request message not currently supported for encoding (" << message->Number << ")" );
        default:
            DmUnknownMessage::Ptr unknownMessage = std::dynamic_pointer_cast< DmUnknownMessage >( message );
            DataBuffer db;
            db.WriteUInt8( SSH_UNKNOWN_MESSAGE );
            db.WriteBuffer( unknownMessage->Payload );
            return DmEncodePacket( db );
    }
}

} // Magenta
