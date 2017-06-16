// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "DraftMiller.h"

#include <Logger.h>

namespace Magenta {

DraftMiller::DraftMiller()
{}

DraftMiller::Ptr DraftMiller::Instance()
{
    static DraftMiller::Ptr instance = DraftMiller::Ptr( new DraftMiller );
    return instance;
}

bool DraftMiller::Register( const DmMessageNumber number, const DraftMiller::MessageHandler& handler )
{
    LOG_WARN << "Handler was previously registered for message number: " << number;
    m_handlerRegistry[ number ] = handler;
    return true;
}

Buffer DraftMiller::HandleMessage( Buffer& buffer )
{
    try
    {
        DmMessage::Ptr request = DmParseMessage( buffer );
        auto it = m_handlerRegistry.find( request->Number );
        if ( it != std::end( m_handlerRegistry ) )
        {
            DmMessage::Ptr response = it->second( request );
            return DmEncodeMessage( response );
        }
        LOG_WARN << "Unhandled message for message number: " << request->Number;
    }
    catch ( const DmIncompletePacketException& ex )
    {
        LOG_INFO << "Fragmented packet received, continuing...";
        throw;
    }
    catch ( const Exception& ex )
    {
        LOG_WARN << "Unexpected error: " << ex.What();
        LOG_WARN << "Returning failure message and continuing...";
    }
    catch ( ... )
    {
        LOG_ERR << "Unhandled exception in message handler";
    }
    return DmEncodeMessage( DmMessage::Ptr( new DmFailure ) );
}

} // Magenta
