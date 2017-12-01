#pragma once

// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <Primitives.h>
#include <Exception.h>

#include <cstdint>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <memory>
#include <map>

namespace DraftMiller {

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
    // May never support...

    // Unknown Message
    SSH_UNKNOWN_MESSAGE                         = 0
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
    DmPacket() :
        Length( 0 )
    {}
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

struct DmDsa
{
    DmDsa()
    {}
    DmDsa( const DmDsa& dsa ) :
        P( dsa.P ), Q( dsa.Q ), G( dsa.G ), Y( dsa.Y ), X( dsa.X )
    {}
    ~DmDsa()
    {}
    Buffer P;
    Buffer Q;
    Buffer G;
    Buffer Y;
    Buffer X;
};

struct DmEcdsa
{
    DmEcdsa()
    {}
    DmEcdsa( const DmEcdsa& ecdsa ) :
        EcdsaCurveName( ecdsa.EcdsaCurveName ), Q( ecdsa.Q ), D( ecdsa.D )
    {}
    ~DmEcdsa()
    {}
    std::string EcdsaCurveName;
    Buffer Q;
    Buffer D;
};

struct DmEd25519
{
    DmEd25519()
    {}
    DmEd25519( const DmEd25519& ed25519 ) :
        EncA( ed25519.EncA ), KEncA( ed25519.KEncA )
    {}
    ~DmEd25519()
    {}
    Buffer EncA;
    Buffer KEncA;
};

struct DmRsa
{
    DmRsa()
    {}
    DmRsa( const DmRsa& rsa ) :
        N( rsa.N ), E( rsa.E ), D( rsa.D ), Iqmp( rsa.Iqmp ), P( rsa.P ), Q( rsa.Q )
    {}
    ~DmRsa()
    {}
    Buffer N;
    Buffer E;
    Buffer D;
    Buffer Iqmp;
    Buffer P;
    Buffer Q;
};

union DmKeyUnion
{
    DmKeyUnion()
    {}
    ~DmKeyUnion()
    {}
    DmDsa Dsa;
    DmEcdsa Ecdsa;
    DmEd25519 Ed25519;
    DmRsa Rsa;
};

struct DmSignature
{
	std::string Algorithm;
	Buffer SignatureBlob;
};

// Request Messages
struct DmMessage
{
    DmMessage( const DmMessageNumber number ) :
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
        DmMessage( SSH_AGENTC_SIGN_REQUEST ),
        Flags( 0 )
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
    ~DmAddIdentity()
    {}
    std::string Type;
    DmKeyUnion Key;
    std::string Comment;
    typedef std::shared_ptr< DmAddIdentity > Ptr;
};

struct DmRemoveIdentity : public DmMessage
{
    DmRemoveIdentity() :
        DmMessage( SSH_AGENTC_REMOVE_IDENTITY )
    {}
    Buffer KeyBlob;
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
    std::string Type;
    DmKeyUnion Key;
    std::string Comment;
    // TODO: Constraints
    typedef std::shared_ptr< DmAddIdentityConstrained > Ptr;
};

struct DmAddSmartCardKey : public DmMessage
{
    DmAddSmartCardKey() :
        DmMessage( SSH_AGENTC_ADD_SMARTCARD_KEY_CONSTRAINED )
    {}
    Buffer Id;
    std::string Pin;
    typedef std::shared_ptr< DmAddSmartCardKey > Ptr;
};

struct DmRemoveSmartCardKey : public DmMessage
{
    DmRemoveSmartCardKey() :
        DmMessage( SSH_AGENTC_REMOVE_SMARTCARD_KEY )
    {}
    Buffer Id;
    std::string Pin;
    typedef std::shared_ptr< DmRemoveSmartCardKey > Ptr;
};

struct DmLock : public DmMessage
{
    DmLock() :
        DmMessage( SSH_AGENTC_LOCK )
    {}
    std::string PassPhrase;
    typedef std::shared_ptr< DmLock > Ptr;
};

struct DmUnlock : public DmMessage
{
    DmUnlock() :
        DmMessage( SSH_AGENTC_UNLOCK )
    {}
    std::string PassPhrase;
    typedef std::shared_ptr< DmUnlock > Ptr;
};

struct DmAddSmartCardKeyConstrained : public DmMessage
{
    DmAddSmartCardKeyConstrained() :
        DmMessage( SSH_AGENTC_ADD_SMARTCARD_KEY_CONSTRAINED )
    {}
    Buffer Id;
    std::string Pin;
    // TODO: Constraints
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
        DmMessage( SSH_AGENT_IDENTITIES_ANSWER ),
        NumberKeys( 0 )
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
	DmSignature Signature;
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
class DmIncompletePacketException : public Exception
{
public:
    DmIncompletePacketException( const std::string& msg ) :
        Exception( msg )
    {}
    virtual ~DmIncompletePacketException()
    {}
};

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
/// @returns A generic DmMessage structure that can be cast to the appropriate message type
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

/// Encode one DmMessage based on the generic DmMessage structure passed in
/// @param message A smart pointer to a generic DmMessage structure
/// @throws DmEncodeException
/// @returns A buffer containing the encoded message
Buffer DmEncodeMessage( const DmMessage::Ptr& message );


/// Singleton class for accessing message handlers.
/// This class will parse a buffer, determine the request message type, then invoke the message handler
/// for that request message.  After the registered handler provides the response, this class will encode
/// the response buffer that may be returned to the client.  You must register handlers for any messages
/// that you would like to support in your agent.
class DraftMillerContext
{
public:
    typedef std::shared_ptr< DraftMillerContext > Ptr;
private:
    DraftMillerContext();
    typedef std::function< DmMessage::Ptr ( const DmMessage::Ptr& ) > MessageHandler;
    std::map< DmMessageNumber, MessageHandler > m_handlerRegistry;
public:
    /// Obtain an instance of the singleton
    /// @returns A singleton instance of the draft miller implementation
    static DraftMillerContext::Ptr Instance();
    /// Register a handler for a particular message type
    /// @param number The number of the message type to register
    /// @param handler The handler to be called when this message is parsed
    /// @returns A bool only so it can do the static registration
    bool Register( const DmMessageNumber number, const MessageHandler& handler );
    /// Handle a request message buffer and obtain a response message buffer
    /// All valid messages will be removed even if a handler is not registered.  They will simply return
    /// a failure message.  If a message cannot be parsed because a complete packet was not found in the
    /// buffer, a DmIncompletePacketException will be thrown.  It should be handled.  You should read
    /// more data and then call this method again.
    /// @param buffer A buffer to be handled, this parameter will be modified as messages are handled
    /// @throws DmParseException, DmEncodeException, DmIncompletePacketException
    Buffer HandleMessage( Buffer& buffer );
};

} // Magenta
