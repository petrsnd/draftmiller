// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "DraftMillerContext.h"

#include <Logger.h>

namespace DraftMiller {

DraftMillerContext::DraftMillerContext()
{}

DraftMillerContext::Ptr DraftMillerContext::Instance()
{
    static DraftMillerContext::Ptr instance = DraftMillerContext::Ptr( new DraftMillerContext );
    return instance;
}

bool DraftMillerContext::Register( const DmMessageNumber number, const DraftMillerContext::MessageHandler& handler )
{
    LOG_WARN << "Handler was previously registered for message number: " << number;
    m_handlerRegistry[ number ] = handler;
    return true;
}

Buffer DraftMillerContext::HandleMessage( Buffer& buffer )
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
    catch ( const DmIncompletePacketException )
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
