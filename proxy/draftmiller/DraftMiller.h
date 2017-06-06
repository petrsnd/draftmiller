#pragma once

// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include <Primitives.h>
#include <Exception.h>

#include <cstdint>
#include <string>
#include <vector>
#include <utility>
#include <memory>

namespace Magenta {

// Definitions
enum DmMessageNumber : uint8_t
{
    SSH_LEGACY_RESERVED_MESSAGE_1               = 1,
    SSH_LEGACY_RESERVED_MESSAGE_2               = 2,
    SSH_LEGACY_RESERVED_MESSAGE_3               = 3,
    SSH_LEGACY_RESERVED_MESSAGE_4               = 4,

    SSH_AGENT_FAILURE                           = 5,
    SSH_AGENT_SUCCESS                           = 6,

    SSH_LEGACY_RESERVED_MESSAGE_7               = 7,
    SSH_LEGACY_RESERVED_MESSAGE_8               = 8,
    SSH_LEGACY_RESERVED_MESSAGE_9               = 9,
    SSH_LEGACY_RESERVED_MESSAGE_10              = 10,

    SSH_AGENTC_REQUEST_IDENTITIES               = 11,
    SSH_AGENT_IDENTITIES_ANSWER                 = 12,

    SSH_AGENTC_SIGN_REQUEST                     = 13,
    SSH_AGENT_SIGN_RESPONSE                     = 14,

    SSH_LEGACY_RESERVED_MESSAGE_15              = 15,
    SSH_LEGACY_RESERVED_MESSAGE_16              = 16,

    SSH_AGENTC_ADD_IDENTITY                     = 17,
    SSH_AGENTC_REMOVE_IDENTITY                  = 18,
    SSH_AGENTC_REMOVE_ALL_IDENTITIES            = 19,

    SSH_AGENTC_ADD_SMARTCARD_KEY                = 20,
    SSH_AGENTC_REMOVE_SMARTCARD_KEY             = 21,

    SSH_AGENTC_LOCK                             = 22,
    SSH_AGENTC_UNLOCK                           = 23,

    SSH_LEGACY_RESERVED_MESSAGE_24              = 24,

    SSH_AGENTC_ADD_ID_CONSTRAINED               = 25,

    SSH_AGENTC_ADD_SMARTCARD_KEY_CONSTRAINED    = 26,

    SSH_AGENTC_EXTENSION                        = 27,
    SSH_AGENT_EXTENSION_FAILURE                 = 28,

    // Known Extensions

};

enum DmContraints
{
    SSH_AGENT_CONSTRAIN_LIFETIME                = 1,
    SSH_AGENT_CONSTRAIN_CONFIRM                 = 2,
    SSH_AGENT_CONSTRAIN_EXTENSION               = 3
};

enum DmSignatureFlags
{
    SSH_LEGACY_RESERVED_FLAG_1                  = 1,
    SSH_AGENT_RSA_SHA2_256                      = 2,
    SSH_AGENT_RSA_SHA2_512                      = 4
};

// Packet
struct DmPacket
{
    uint32_t Length;
    Buffer Contents;
};

// Message Parts
struct DmKey
{
    Buffer KeyBlob;
    std::string Comment;
    typedef std::vector< DmKey > List;
};

// Request Messages
struct DmMessage
{
    DmMessage(const DmMessageNumber number) :
        Number( number )
    {}
    virtual ~DmMessage()
    {}
    const DmMessageNumber Number;
    typedef std::shared_ptr< DmMessage > Ptr;
};

struct DmRequestIdentities : public DmMessage
{
    DmRequestIdentities() :
        DmMessage( SSH_AGENTC_REQUEST_IDENTITIES )
    {}
    typedef std::shared_ptr< DmRequestIdentities > Ptr;
};

struct DmSignRequest : public DmMessage
{
    DmSignRequest() :
        DmMessage( SSH_AGENTC_SIGN_REQUEST )
    {}
    Buffer KeyBlob;
    Buffer Data;
    uint32_t Flags;
    typedef std::shared_ptr< DmSignRequest > Ptr;
};

struct DmAddIdentity : public DmMessage
{
    DmAddIdentity() :
        DmMessage( SSH_AGENTC_ADD_IDENTITY )
    {}
    typedef std::shared_ptr< DmAddIdentity > Ptr;
};

struct DmRemoveIdentity : public DmMessage
{
    DmRemoveIdentity() :
        DmMessage( SSH_AGENTC_REMOVE_IDENTITY )
    {}
    typedef std::shared_ptr< DmRemoveIdentity > Ptr;
};

struct DmRemoveAllIdentities : public DmMessage
{
    DmRemoveAllIdentities() :
        DmMessage( SSH_AGENTC_REMOVE_ALL_IDENTITIES )
    {}
    typedef std::shared_ptr< DmRemoveAllIdentities > Ptr;
};

struct DmAddIdentityConstrained : public DmMessage
{
    DmAddIdentityConstrained() :
        DmMessage( SSH_AGENTC_ADD_ID_CONSTRAINED )
    {}
    typedef std::shared_ptr< DmAddIdentityConstrained > Ptr;
};

struct DmAddSmartCardKey : public DmMessage
{
    DmAddSmartCardKey() :
        DmMessage( SSH_AGENTC_ADD_SMARTCARD_KEY_CONSTRAINED )
    {}
    typedef std::shared_ptr< DmAddSmartCardKey > Ptr;
};

struct DmRemoveSmartCardKey : public DmMessage
{
    DmRemoveSmartCardKey() :
        DmMessage( SSH_AGENTC_REMOVE_SMARTCARD_KEY )
    {}
    typedef std::shared_ptr< DmRemoveSmartCardKey > Ptr;
};

struct DmLock : public DmMessage
{
    DmLock() :
        DmMessage( SSH_AGENTC_LOCK )
    {}
    typedef std::shared_ptr< DmLock > Ptr;
};

struct DmUnlock : public DmMessage
{
    DmUnlock() :
        DmMessage( SSH_AGENTC_UNLOCK )
    {}
    typedef std::shared_ptr< DmUnlock > Ptr;
};

struct DmAddSmartCardKeyConstrained : public DmMessage
{
    DmAddSmartCardKeyConstrained() :
        DmMessage( SSH_AGENTC_ADD_SMARTCARD_KEY_CONSTRAINED )
    {}
    typedef std::shared_ptr< DmAddSmartCardKeyConstrained > Ptr;
};

struct DmExtension : public DmMessage
{
    DmExtension() :
        DmMessage( SSH_AGENTC_EXTENSION )
    {}
    typedef std::shared_ptr< DmExtension > Ptr;
};

// Response Messages
struct DmFailure : public DmMessage
{
    DmFailure() :
        DmMessage( SSH_AGENT_FAILURE )
    {}
    typedef std::shared_ptr< DmFailure > Ptr;
};

struct DmSuccess : public DmMessage
{
    DmSuccess() :
        DmMessage( SSH_AGENT_SUCCESS )
    {}
    typedef std::shared_ptr< DmSuccess > Ptr;
};

struct DmIdentitiesAnswer : public DmMessage
{
    DmIdentitiesAnswer() :
        DmMessage( SSH_AGENT_IDENTITIES_ANSWER )
    {}
    uint32_t NumberKeys;
    DmKey::List Keys;
    typedef std::shared_ptr< DmIdentitiesAnswer > Ptr;
};

struct DmSignResponse : public DmMessage
{
    DmSignResponse() :
        DmMessage( SSH_AGENT_SIGN_RESPONSE )
    {}
    Buffer Signature;
    typedef std::shared_ptr< DmSignResponse > Ptr;
};

struct DmExtensionFailure : public DmMessage
{
    DmExtensionFailure() :
        DmMessage( SSH_AGENT_EXTENSION_FAILURE )
    {}
    typedef std::shared_ptr< DmExtensionFailure > Ptr;
};

// Unknown Message
struct DmUnknownMessage : public DmMessage
{
    DmUnknownMessage( const DmMessageNumber number ) :
        DmMessage( number )
    {}
    Buffer Payload;
    typedef std::shared_ptr< DmUnknownMessage > Ptr;
};

// API
class DmParseException : public Exception
{
public:
    DmParseException( const std::string& msg ) :
        Exception( msg )
    {}
    virtual ~DmParseException()
    {}
};

/// Parse one DmMessage from the buffer and modify the buffer to remove those bytes
/// @param buffer A buffer containing at least one DmMessage
/// @throws DmParseException
DmMessage::Ptr DmParseMessage( Buffer& buffer );


class DmEncodeException : public Exception
{
public:
    DmEncodeException( const std::string& msg ) :
        Exception( msg )
    {}
    virtual ~DmEncodeException()
    {}
};

/// Encode a DmIdentitiesAnswer message as a buffer
/// @param identitiesAnswer Shared pointer to a DmIdentitiesAnswer structure
Buffer DmEncodeIdentitiesAnswer( const DmIdentitiesAnswer::Ptr& identitiesAnswer );

/// Encode a DmSignResponse message as a buffer
/// @param signResponse Shared pointer to a DmSignResponse structure
Buffer DmEncodeSignResponse( const DmSignResponse::Ptr& signResponse );

// Matt --
// I don't like this interface, because I give you back a DmMessage, and you have to look at
// the number field to know what it is and then dynamic_cast it to the right message type
// The interface we should have is for you to register handlers for each of the message types

} // Magenta
