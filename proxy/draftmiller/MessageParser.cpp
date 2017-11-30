// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "DraftMiller.h"

#include <DataBuffer.h>
#include <StringUtils.h>

namespace Magenta {

// Parse primitives
static Buffer DmParseBuffer( DataBuffer& db )
{
    uint32_t size = db.ReadUInt32();
    return db.ReadBuffer( size );
}

static std::string DmParseString( DataBuffer& db )
{
    uint32_t size = db.ReadUInt32();
    return db.ReadString( size );
}

// Parser helpers
static void DmParseKeyUnion( DataBuffer& db, DmKeyUnion& key, const std::string& type )
{
    if ( type == "ssh-dss" )
    {
        key.Dsa.P = DmParseBuffer( db );
        key.Dsa.Q = DmParseBuffer( db );
        key.Dsa.G = DmParseBuffer( db );
        key.Dsa.Y = DmParseBuffer( db );
        key.Dsa.X = DmParseBuffer( db );
    }
    else if ( type == "ssh-ed25519" )
    {
        key.Ed25519.EncA = DmParseBuffer( db  );
        key.Ed25519.KEncA = DmParseBuffer( db );
    }
    else if ( type == "ssh-rsa" )
    {
        key.Rsa.N = DmParseBuffer( db );
        key.Rsa.E = DmParseBuffer( db );
        key.Rsa.D = DmParseBuffer( db );
        key.Rsa.Iqmp = DmParseBuffer( db );
        key.Rsa.P = DmParseBuffer( db );
        key.Rsa.Q = DmParseBuffer( db );
    }
    else // Assume ECDSA -- check for curve name
    {
        key.Ecdsa.EcdsaCurveName = DmParseString( db );
        key.Ecdsa.Q = DmParseBuffer( db );
        key.Ecdsa.D = DmParseBuffer( db );
    }
}

static void DmParseSignature(DataBuffer& db, DmSignature& signature)
{
	db.ReadInt32(); // Ignore the encapsulating buffer length
	signature.Algorithm = DmParseString(db);
	signature.SignatureBlob = DmParseBuffer(db);
}

// Parse requests
static DmMessage::Ptr DmParseSignRequest( DataBuffer& db )
{
    DmSignRequest::Ptr signRequest = std::make_shared< DmSignRequest >();
    signRequest->KeyBlob = DmParseBuffer( db );
    signRequest->Data = DmParseBuffer( db );
    if ( db.Size() >= 4 )
    {
        // I have seen test buffers that just leave this off, so I
        // check to see if there are any bytes left in the packet
        signRequest->Flags = db.ReadUInt32();
    }
    return signRequest;
}

static DmMessage::Ptr DmParseAddIdentity( DataBuffer& db )
{
    DmAddIdentity::Ptr addIdentity = std::make_shared< DmAddIdentity >();
    addIdentity->Type = DmParseString( db );
    DmParseKeyUnion( db, addIdentity->Key, addIdentity->Type );
    addIdentity->Comment = DmParseString( db );
    return addIdentity;
}

static DmMessage::Ptr DmParseAddIdentityConstrained( DataBuffer& db )
{
    DmAddIdentityConstrained::Ptr addIdentityConstrained = std::make_shared< DmAddIdentityConstrained >();
    addIdentityConstrained->Type = DmParseString( db );
    DmParseKeyUnion( db, addIdentityConstrained->Key, addIdentityConstrained->Type );
    addIdentityConstrained->Comment = DmParseString( db );
    // TODO: constraints
    return addIdentityConstrained;
}

static DmMessage::Ptr DmParseRemoveIdentity( DataBuffer& db )
{
    DmRemoveIdentity::Ptr removeIdentity = std::make_shared< DmRemoveIdentity >();
    removeIdentity->KeyBlob = DmParseBuffer( db );
    return removeIdentity;
}

static DmMessage::Ptr DmParseAddSmartCardKey( DataBuffer& db )
{
    DmAddSmartCardKey::Ptr addSmartCardKey = std::make_shared< DmAddSmartCardKey >();
    addSmartCardKey->Id = DmParseBuffer( db );
    addSmartCardKey->Pin = DmParseString( db );
    return addSmartCardKey;
}

static DmMessage::Ptr DmParseAddSmartCardKeyConstrained( DataBuffer& db )
{
    DmAddSmartCardKeyConstrained::Ptr addSmartCardKeyConstrained = std::make_shared< DmAddSmartCardKeyConstrained >();
    addSmartCardKeyConstrained->Id = DmParseBuffer( db );
    addSmartCardKeyConstrained->Pin = DmParseString( db );
    // TODO: Constraints
    return addSmartCardKeyConstrained;
}

static DmMessage::Ptr DmParseRemoveSmartCardKey( DataBuffer& db )
{
    DmRemoveSmartCardKey::Ptr removeSmartCardKey = std::make_shared< DmRemoveSmartCardKey >();
    removeSmartCardKey->Id = DmParseBuffer( db );
    removeSmartCardKey->Pin = DmParseString( db );
    return removeSmartCardKey;
}

static DmMessage::Ptr DmParseLock( DataBuffer& db )
{
    DmLock::Ptr lock = std::make_shared< DmLock >();
    lock->PassPhrase = DmParseString( db );
    return lock;
}

static DmMessage::Ptr DmParseUnlock( DataBuffer& db )
{
    DmUnlock::Ptr unlock = std::make_shared< DmUnlock >();
    unlock->PassPhrase = DmParseString( db );
    return unlock;
}

// Parse responses
static DmMessage::Ptr DmParseIdentitiesAnswer( DataBuffer& db )
{
    DmIdentitiesAnswer::Ptr identitiesAnswer = std::make_shared< DmIdentitiesAnswer >();
    identitiesAnswer->NumberKeys = db.ReadUInt32();
    for ( uint32_t i = 0; i < identitiesAnswer->NumberKeys; i++ )
    {
        DmKey key;
        key.KeyBlob = DmParseBuffer( db );
        key.Comment = DmParseString( db );
        identitiesAnswer->Keys.push_back( key );
    }
    return identitiesAnswer;
}

static DmMessage::Ptr DmParseSignResponse( DataBuffer& db )
{
    DmSignResponse::Ptr signResponse = std::make_shared< DmSignResponse >();
	DmParseSignature(db, signResponse->Signature);
    return signResponse;
}


static DmMessage::Ptr DmParseMessageInternal( DataBuffer& db )
{
    uint8_t messageNumber = db.ReadUInt8();
    switch ( static_cast< DmMessageNumber >( messageNumber ) )
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
            throw DmParseException( SC() << "Legacy message not supported (" << messageNumber << ")" );

        // Requests
        case SSH_AGENTC_REQUEST_IDENTITIES:
            return std::make_shared< DmRequestIdentities >();
        case SSH_AGENTC_SIGN_REQUEST:
            return DmParseSignRequest( db );
        case SSH_AGENTC_ADD_IDENTITY:
            return DmParseAddIdentity( db );
        case SSH_AGENTC_REMOVE_IDENTITY:
            return DmParseRemoveIdentity( db );
        case SSH_AGENTC_REMOVE_ALL_IDENTITIES:
            return std::make_shared< DmRemoveAllIdentities >();
        case SSH_AGENTC_ADD_SMARTCARD_KEY:
            return DmParseAddSmartCardKey( db );
        case SSH_AGENTC_REMOVE_SMARTCARD_KEY:
            return DmParseRemoveSmartCardKey( db );
        case SSH_AGENTC_LOCK:
            return DmParseLock( db );
        case SSH_AGENTC_UNLOCK:
            return DmParseUnlock( db );
        case SSH_AGENTC_ADD_ID_CONSTRAINED:
            return DmParseAddIdentityConstrained( db );
        case SSH_AGENTC_ADD_SMARTCARD_KEY_CONSTRAINED:
            return DmParseAddSmartCardKeyConstrained( db );
        case SSH_AGENTC_EXTENSION:
            throw DmParseException(
                SC() << "Request message not currently supported for parsing (" << messageNumber << ")" );

        // Responses
        case SSH_AGENT_FAILURE:
            return std::make_shared< DmFailure >();
        case SSH_AGENT_SUCCESS:
            return std::make_shared< DmSuccess >();
        case SSH_AGENT_IDENTITIES_ANSWER:
            return DmParseIdentitiesAnswer( db );
        case SSH_AGENT_SIGN_RESPONSE:
            return DmParseSignResponse( db );
        case SSH_AGENT_EXTENSION_FAILURE:
            throw DmParseException(
                SC() << "Response message not currently supported for parsing (" << messageNumber << ")" );

        default:
            DmUnknownMessage::Ptr unknownMessage = std::make_shared< DmUnknownMessage >(
                static_cast< DmMessageNumber >( messageNumber ) );
            unknownMessage->Payload = db.ReadBuffer();
            return unknownMessage;
    }
}


DmMessage::Ptr DmParseMessage( Buffer& buffer )
{
    try
    {
        size_t bytesRemoved = 0;
        DataBuffer db( buffer );
        uint32_t packetSize = db.ReadUInt32();
        if ( db.Size() < packetSize )
        {
            throw DmIncompletePacketException(
                SC() << "Buffer does not contain enough bytes for packet size: " << packetSize );
        }
        // Create a new DataBuffer to ensure only the correct number of bytes are parsed
        DataBuffer dbMessage( db.ReadBuffer( packetSize ) );
        DmMessage::Ptr parsedMessage = DmParseMessageInternal( dbMessage );
        bytesRemoved = sizeof( uint32_t ) + packetSize;
        buffer.erase( buffer.begin(), buffer.begin() + bytesRemoved );
        return parsedMessage;
    }
    catch ( const Exception& ex )
    {
        throw DmParseException( ex.What() );
    }
}

} // Magenta
