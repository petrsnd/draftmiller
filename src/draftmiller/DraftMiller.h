#pragma once

// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include <Primitives.h>

#include <cstdint>

namespace Magenta {

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

struct DmMessage
{
    uint32_t Length;
    Buffer Contents;
};

// Message Parts
// TODO: some will need typedef for lists of type vector< T >

// Request Messages
struct DmRequestIdentities
{
    const DmMessageNumber Number = SSH_AGENTC_REQUEST_IDENTITIES;

};

struct DmSignRequest
{
    const DmMessageNumber Number = SSH_AGENTC_SIGN_REQUEST;

};

struct DmAddIdentity
{
    const DmMessageNumber Number = SSH_AGENTC_ADD_IDENTITY;

};

struct DmRemoveIdentity
{
    const DmMessageNumber Number = SSH_AGENTC_REMOVE_IDENTITY;

};

struct DmRemoveAllIdentities
{
    const DmMessageNumber Number = SSH_AGENTC_REMOVE_ALL_IDENTITIES;

};

struct DmAddIdentityConstrained
{
    const DmMessageNumber Number = SSH_AGENTC_ADD_ID_CONSTRAINED;

};

struct DmAddSmartCardKey
{
    const DmMessageNumber Number = SSH_AGENTC_ADD_SMARTCARD_KEY_CONSTRAINED;

};

struct DmRemoveSmartCardKey
{
    const DmMessageNumber Number = SSH_AGENTC_REMOVE_SMARTCARD_KEY;
};

struct DmLock
{
    const DmMessageNumber Number = SSH_AGENTC_LOCK;

};

struct DmUnlock
{
    const DmMessageNumber Number = SSH_AGENTC_UNLOCK;

};

struct DmAddSmartCardKeyConstrained
{
    const DmMessageNumber Number = SSH_AGENTC_ADD_SMARTCARD_KEY_CONSTRAINED;

};

struct DmExtension
{
    const DmMessageNumber Number = SSH_AGENTC_EXTENSION;

};

// Response Messages
struct DmFailure
{
    const DmMessageNumber Number = SSH_AGENT_FAILURE;

};

struct DmSuccess
{
    const DmMessageNumber Number = SSH_AGENT_SUCCESS;

};

struct DmIdentitiesAnswer
{
    const DmMessageNumber Number = SSH_AGENT_IDENTITIES_ANSWER;

};

struct DmSignResponse
{
    const DmMessageNumber Number = SSH_AGENT_SIGN_RESPONSE;

};

struct DmExtensionFailure
{
    const DmMessageNumber Number = SSH_AGENT_EXTENSION_FAILURE;
    
};

// Unknown Message
struct DmUnknownMessage
{
    uint8_t Number;
    Buffer Payload;
};

} // Magenta
