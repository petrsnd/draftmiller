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

static void DmEncodeBuffer( DataBuffer &db, const Buffer &buf )
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
static void DmEncodeKeyUnion( DataBuffer &db, const std::string &type, const DmKeyUnion &key )
{
    if ( type == "ssh-dss" )
    {
        DmEncodeBuffer( db, key.Dsa.P );
        DmEncodeBuffer( db, key.Dsa.Q );
        DmEncodeBuffer( db, key.Dsa.G );
        DmEncodeBuffer( db, key.Dsa.Y );
        DmEncodeBuffer( db, key.Dsa.X );
    }
    else if ( type == "ssh-ed25519" )
    {
        DmEncodeBuffer( db, key.Ed25519.EncA );
        DmEncodeBuffer( db, key.Ed25519.KEncA );
    }
    else if ( type == "ssh-rsa" )
    {
        DmEncodeBuffer( db, key.Rsa.N );
        DmEncodeBuffer( db, key.Rsa.E );
        DmEncodeBuffer( db, key.Rsa.D );
        DmEncodeBuffer( db, key.Rsa.Iqmp );
        DmEncodeBuffer( db, key.Rsa.P );
        DmEncodeBuffer( db, key.Rsa.Q );
    }
    else // Assume ECDSA -- check for curve name
    {
        DmEncodeString( db, key.Ecdsa.EcdsaCurveName );
        DmEncodeBuffer( db, key.Ecdsa.Q );
        DmEncodeBuffer( db, key.Ecdsa.D );
    }
}

static void DmEncodeSignature(DataBuffer &db, const DmSignature& signature)
{
	DataBuffer signatureBlob;
	DmEncodeString(signatureBlob, signature.Algorithm);
	DmEncodeBuffer(signatureBlob, signature.SignatureBlob);
	DmEncodeBuffer(db, signatureBlob);
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
    DmEncodeBuffer( db, signRequest->KeyBlob );
    DmEncodeBuffer( db, signRequest->Data );
    if ( signRequest->Flags != 0 )
    {
        // The only implementation I have sample data from leaves this off,
        // so I'm checking for zero and just not including it if it is empty
        db.WriteUInt32( signRequest->Flags );
    }
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
    DmEncodeKeyUnion( db, addIdentity->Type, addIdentity->Key );
    DmEncodeString( db, addIdentity->Comment );
    return DmEncodePacket( db );
}

static Buffer DmEncodeAddIdentityConstrained( const DmAddIdentityConstrained::Ptr& addIdentityConstrained )
{
    if ( addIdentityConstrained == nullptr )
    {
        throw DmEncodeException( "Add identity constrained structure cannot be null" );
    }
    DataBuffer db;
    db.WriteUInt8( addIdentityConstrained->Number );
    DmEncodeString( db, addIdentityConstrained->Type );
    DmEncodeKeyUnion( db, addIdentityConstrained->Type, addIdentityConstrained->Key );
    DmEncodeString( db, addIdentityConstrained->Comment );
    // TODO: constraints
    return DmEncodePacket( db );
}

static Buffer DmEncodeRemoveIdentity( const DmRemoveIdentity::Ptr& removeIdentity )
{
    if ( removeIdentity == nullptr )
    {
        throw DmEncodeException( "Remove identity structure cannot be null" );
    }
    DataBuffer db;
    db.WriteUInt8( removeIdentity->Number );
    DmEncodeBuffer( db, removeIdentity->KeyBlob );
    return DmEncodePacket( db );
}

static Buffer DmEncodeRemoveAllIdentities( const DmRemoveAllIdentities::Ptr& removeAllIdentities )
{
    return DmEncodeSimpleMessage( removeAllIdentities, "Remove all identities" );
}

static Buffer DmEncodeAddSmartCardKey( const DmAddSmartCardKey::Ptr& addSmartCardKey )
{
    if ( addSmartCardKey == nullptr )
    {
        throw DmEncodeException( "Add smart card key structure cannot be null" );
    }
    DataBuffer db;
    db.WriteUInt8( addSmartCardKey->Number );
    DmEncodeBuffer( db, addSmartCardKey->Id );
    DmEncodeString( db, addSmartCardKey->Pin );
    return DmEncodePacket( db );
}

static Buffer DmEncodeAddSmartCardKeyConstrained( const DmAddSmartCardKeyConstrained::Ptr& addSmartCardKeyConstrained )
{
    if ( addSmartCardKeyConstrained == nullptr )
    {
        throw DmEncodeException( "Add smart card key constrained structure cannot be null" );
    }
    DataBuffer db;
    db.WriteUInt8( addSmartCardKeyConstrained->Number );
    DmEncodeBuffer( db, addSmartCardKeyConstrained->Id );
    DmEncodeString( db, addSmartCardKeyConstrained->Pin );
    // TODO: Constraints
    return DmEncodePacket( db );
}

static Buffer DmEncodeRemoveSmartCardKey( const DmRemoveSmartCardKey::Ptr& removeSmartCardKey )
{
    if ( removeSmartCardKey == nullptr )
    {
        throw DmEncodeException( "Remove smart card key structure cannot be null" );
    }
    DataBuffer db;
    db.WriteUInt8( removeSmartCardKey->Number );
    DmEncodeBuffer( db, removeSmartCardKey->Id );
    DmEncodeString( db, removeSmartCardKey->Pin );
    return DmEncodePacket( db );
}

static Buffer DmEncodeLock( const DmLock::Ptr& lock )
{
    if ( lock == nullptr )
    {
        throw DmEncodeException( "Lock structure cannot be null" );
    }
    DataBuffer db;
    db.WriteUInt8( lock->Number );
    DmEncodeString( db, lock->PassPhrase );
    return DmEncodePacket( db );
}

static Buffer DmEncodeUnlock( const DmUnlock::Ptr& unlock )
{
    if ( unlock == nullptr )
    {
        throw DmEncodeException( "Unlock structure cannot be null" );
    }
    DataBuffer db;
    db.WriteUInt8( unlock->Number );
    DmEncodeString( db, unlock->PassPhrase );
    return DmEncodePacket( db );
}

// Encode responses
static Buffer DmEncodeFailure()
{
    return DmEncodeSimpleMessage( std::make_shared< DmMessage >( SSH_AGENT_FAILURE ), "Failure" );
}

static Buffer DmEncodeSuccess()
{
    return DmEncodeSimpleMessage( std::make_shared< DmMessage >( SSH_AGENT_SUCCESS ), "Success" );
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
    for ( uint32_t i = 0; i < identitiesAnswer->NumberKeys; i++ )
    {
        DmEncodeBuffer( db, identitiesAnswer->Keys[i].KeyBlob );
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
	DmEncodeSignature(db, signResponse->Signature );
    return DmEncodePacket( db );
}

static Buffer DmEncodeExtensionFailure( const DmExtensionFailure::Ptr& extensionFailure )
{
    return DmEncodeSimpleMessage( extensionFailure, "Extension failure" );
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
            return DmEncodeRemoveIdentity( std::dynamic_pointer_cast< DmRemoveIdentity >( message ) );
        case SSH_AGENTC_REMOVE_ALL_IDENTITIES:
            return DmEncodeRemoveAllIdentities( std::dynamic_pointer_cast< DmRemoveAllIdentities >( message ) );
        case SSH_AGENTC_ADD_SMARTCARD_KEY:
            return DmEncodeAddSmartCardKey( std::dynamic_pointer_cast< DmAddSmartCardKey >( message ) );
        case SSH_AGENTC_REMOVE_SMARTCARD_KEY:
            return DmEncodeRemoveSmartCardKey( std::dynamic_pointer_cast< DmRemoveSmartCardKey >( message ) );
        case SSH_AGENTC_LOCK:
            return DmEncodeLock( std::dynamic_pointer_cast< DmLock >( message ) );
        case SSH_AGENTC_UNLOCK:
            return DmEncodeUnlock( std::dynamic_pointer_cast< DmUnlock >( message ) );
        case SSH_AGENTC_ADD_ID_CONSTRAINED:
            return DmEncodeAddIdentityConstrained( std::dynamic_pointer_cast< DmAddIdentityConstrained >( message ) );
        case SSH_AGENTC_ADD_SMARTCARD_KEY_CONSTRAINED:
            return DmEncodeAddSmartCardKeyConstrained( std::dynamic_pointer_cast< DmAddSmartCardKeyConstrained >( message ) );
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
