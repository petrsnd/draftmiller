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

// Encode helpers
static void DmEncodeKeyT( DataBuffer& db, const std::string& type, const DmKeyUnion& key )
{
    if ( type == "ssh-dss" )
    {
        DmEncodeString( db, key.Dsa.P );
        DmEncodeString( db, key.Dsa.Q );
        DmEncodeString( db, key.Dsa.G );
        DmEncodeString( db, key.Dsa.Y );
        DmEncodeString( db, key.Dsa.X );
    }
    else if ( type == "ssh-ed25519" )
    {
        DmEncodeString( db, key.Ed25519.EncA );
        DmEncodeString( db, key.Ed25519.KEncA );
    }
    else if ( type == "ssh-rsa" )
    {
        DmEncodeString( db, key.Rsa.N );
        DmEncodeString( db, key.Rsa.E );
        DmEncodeString( db, key.Rsa.D );
        DmEncodeString( db, key.Rsa.Iqmp );
        DmEncodeString( db, key.Rsa.P );
        DmEncodeString( db, key.Rsa.Q );
    }
    else // Assume ECDSA -- check for curve name
    {
        DmEncodeString( db, key.Ecdsa.EcdsaCurveName );
        DmEncodeString( db, key.Ecdsa.Q );
        DmEncodeString( db, key.Ecdsa.D );
    }
}

static Buffer DmEncodeSimpleMessage( const DmMessage::Ptr& message, const std::string& name )
{
    if ( message == nullptr )
    {
        throw DmEncodeException( SC() << " structure cannot be null" );
    }
    DataBuffer db;
    db.WriteUInt8( message->Number );
    return DmEncodePacket( db );
}

// Encode requests
static Buffer DmEncodeRequestIdentities( const DmRequestIdentities::Ptr& requestIdentities )
{
    return DmEncodeSimpleMessage( requestIdentities, "Request identities" );
}

static Buffer DmEncodeSignRequest( const DmSignRequest::Ptr& signRequest )
{
    if ( signRequest == nullptr )
    {
        throw DmEncodeException( "Sign request structure cannot be null" );
    }
    DataBuffer db;
    db.WriteUInt8( signRequest->Number );
    DmEncodeString( db, signRequest->KeyBlob );
    DmEncodeString( db, signRequest->Data );
    db.WriteUInt32( signRequest->Flags );
    return DmEncodePacket( db );
}

static Buffer DmEncodeAddIdentity( const DmAddIdentity::Ptr& addIdentity )
{
    if ( addIdentity == nullptr )
    {
        throw DmEncodeException( "Add identity structure cannot be null" );
    }
    DataBuffer db;
    db.WriteUInt8( addIdentity->Number );
    DmEncodeString( db, addIdentity->Type );
    DmEncodeKeyT( db, addIdentity->Type, addIdentity->Key );
    DmEncodeString( db, addIdentity->Comment );
    return DmEncodePacket( db );
}

static Buffer DmEncodeAddIdentityConstrained( const DmAddIdentityConstrained::Ptr& addIdentityConstrained )
{
    if ( addIdentityConstrained == nullptr )
    {
        throw DmEncodeException( "Add identity structure cannot be null" );
    }
    DataBuffer db;
    db.WriteUInt8( addIdentityConstrained->Number );
    DmEncodeString( db, addIdentityConstrained->Type );
    DmEncodeKeyT( db, addIdentityConstrained->Type, addIdentityConstrained->Key );
    DmEncodeString( db, addIdentityConstrained->Comment );
    // TODO: constraints
    return DmEncodePacket( db );
}

static Buffer DmEncodeRemoveAllIdentities( const DmRemoveAllIdentities::Ptr& removeAllIdentities )
{
    return DmEncodeSimpleMessage( removeAllIdentities, "Remove all identities" );
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

static Buffer DmEncodeExtensionFailure( const DmExtensionFailure::Ptr& extensionFailure )
{
    if ( extensionFailure == nullptr )
    {
        throw DmEncodeException( "Extension failure structure cannot be null" );
    }
    DataBuffer db;
    db.WriteUInt8( extensionFailure->Number );
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

        // Requests
        case SSH_AGENTC_REQUEST_IDENTITIES:
            return DmEncodeRequestIdentities( std::dynamic_pointer_cast< DmRequestIdentities >( message ) );
        case SSH_AGENTC_SIGN_REQUEST:
            return DmEncodeSignRequest( std::dynamic_pointer_cast< DmSignRequest >( message ) );
        case SSH_AGENTC_ADD_IDENTITY:
            return DmEncodeAddIdentity( std::dynamic_pointer_cast< DmAddIdentity >( message ) );
        case SSH_AGENTC_REMOVE_IDENTITY:
            throw DmEncodeException(
                SC() << "Request message not currently supported for encoding (" << message->Number << ")" );
        case SSH_AGENTC_REMOVE_ALL_IDENTITIES:
            return DmEncodeRemoveAllIdentities( std::dynamic_pointer_cast< DmRemoveAllIdentities >( message ) );
        case SSH_AGENTC_ADD_SMARTCARD_KEY:
        case SSH_AGENTC_REMOVE_SMARTCARD_KEY:
        case SSH_AGENTC_LOCK:
        case SSH_AGENTC_UNLOCK:
            throw DmEncodeException(
                SC() << "Request message not currently supported for encoding (" << message->Number << ")" );
        case SSH_AGENTC_ADD_ID_CONSTRAINED:
            return DmEncodeAddIdentityConstrained( std::dynamic_pointer_cast< DmAddIdentityConstrained >( message ) );
        case SSH_AGENTC_ADD_SMARTCARD_KEY_CONSTRAINED:
        case SSH_AGENTC_EXTENSION:
            throw DmEncodeException(
                SC() << "Request message not currently supported for encoding (" << message->Number << ")" );

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
            return DmEncodeExtensionFailure( std::dynamic_pointer_cast< DmExtensionFailure >( message ) );

        default:
            DmUnknownMessage::Ptr unknownMessage = std::dynamic_pointer_cast< DmUnknownMessage >( message );
            DataBuffer db;
            db.WriteUInt8( SSH_UNKNOWN_MESSAGE );
            db.WriteBuffer( unknownMessage->Payload );
            return DmEncodePacket( db );
    }
}

} // Magenta
