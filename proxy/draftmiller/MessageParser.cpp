// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "DraftMiller.h"

#include <DataBuffer.h>
#include <StringUtils.h>

namespace Magenta {

static Buffer DmParseString( DataBuffer& db )
{
    uint32_t size = db.ReadUInt32();
    return db.ReadBuffer( size );
}

static DmMessage::Ptr DmParseSignRequest( DataBuffer& db )
{
    DmSignRequest::Ptr signRequest = std::make_shared< DmSignRequest >();
    signRequest->KeyBlob = DmParseString( db );
    signRequest->Data = DmParseString( db );
    signRequest->Flags = db.ReadUInt32();
    return signRequest;
}

static DmMessage::Ptr DmParseMessageInternal( DataBuffer& db, const uint32_t size)
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

        // Responses
        case SSH_AGENT_FAILURE:
            return std::make_shared< DmFailure >();
        case SSH_AGENT_SUCCESS:
            return std::make_shared< DmSuccess >();
        case SSH_AGENT_IDENTITIES_ANSWER:
        case SSH_AGENT_SIGN_RESPONSE:
        case SSH_AGENT_EXTENSION_FAILURE:
            throw DmParseException(
                SC() << "Response message not currently supported for parsing (" << messageNumber << ")" );

        // Requests
        case SSH_AGENTC_REQUEST_IDENTITIES:
            return std::make_shared< DmRequestIdentities >();
        case SSH_AGENTC_SIGN_REQUEST:
            return DmParseSignRequest( db );

        case SSH_AGENTC_ADD_IDENTITY:
        case SSH_AGENTC_REMOVE_IDENTITY:
            throw DmParseException(
                SC() << "Request message not currently supoorted for parsing (" << messageNumber << ")" );
        case SSH_AGENTC_REMOVE_ALL_IDENTITIES:
            return std::make_shared< DmRemoveAllIdentities >();
        case SSH_AGENTC_ADD_SMARTCARD_KEY:
        case SSH_AGENTC_REMOVE_SMARTCARD_KEY:
        case SSH_AGENTC_LOCK:
        case SSH_AGENTC_UNLOCK:
        case SSH_AGENTC_ADD_ID_CONSTRAINED:
        case SSH_AGENTC_ADD_SMARTCARD_KEY_CONSTRAINED:
        case SSH_AGENTC_EXTENSION:
            throw DmParseException(
                SC() << "Request message not currently supported for parsing (" << messageNumber << ")" );
        default:
            DmUnknownMessage::Ptr unknownMessage = std::make_shared< DmUnknownMessage >(
                static_cast< DmMessageNumber >( messageNumber ) );
            unknownMessage->Payload = db.ReadBuffer( size );
            return unknownMessage;
    }
}


DmMessage::Ptr DmParseMessage( Buffer& buffer )
{
    try
    {
        size_t bytesRemoved = 0;
        DataBuffer db(buffer);
        uint32_t packetSize = db.ReadUInt32();
        if ( db.Size() < packetSize )
        {
            throw DmParseException(
                SC() << "Buffer does not contain enough bytes for packet size: " << packetSize );
        }
        DmMessage::Ptr parsedMessage = DmParseMessageInternal( db, packetSize );
        bytesRemoved = sizeof( uint32_t ) + packetSize;
        buffer.erase( buffer.begin(), buffer.begin() + bytesRemoved );
        return parsedMessage;
    }
    catch ( Exception& ex )
    {
        throw DmParseException( ex.What() );
    }
}

} // Magenta
