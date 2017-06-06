// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "DraftMiller.h"

#include <DataBuffer.h>
#include <StringUtils.h>

namespace Magenta {

static void DmEncodeString( DataBuffer& db, const std::string& str )
{
    db.WriteUInt32( str.length() );
    db.WriteString( str, false );
}

static void DmEncodeString( DataBuffer& db, const Buffer& buf )
{
    db.WriteUInt32( buf.size() );
    db.WriteBuffer( buf );
}

static Buffer DmEncodePacket( const DataBuffer& db )
{
    DataBuffer packet;
    packet.WriteUInt32( db.Size() );
    packet.WriteBuffer( db );
    return packet;
}


Buffer DmEncodeFailure()
{
    DataBuffer db;
    db.WriteUInt8( SSH_AGENT_FAILURE );
    return DmEncodePacket( db );
}

Buffer DmEncodeSuccess()
{
    DataBuffer db;
    db.WriteUInt8( SSH_AGENT_SUCCESS );
    return DmEncodePacket( db );
}

Buffer DmEncodeIdentitiesAnswer( const DmIdentitiesAnswer::Ptr& identitiesAnswer )
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

Buffer DmEncodeSignResponse( const DmSignResponse::Ptr& signResponse )
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

} // Magenta
