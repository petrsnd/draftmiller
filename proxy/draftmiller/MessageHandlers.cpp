// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "DraftMiller.h"

namespace Magenta {

DraftMiller::Ptr DraftMiller::Instance()
{
    static DraftMiller::Ptr instance = std::make_shared< DraftMiller >();
    return instance;
}

bool DraftMiller::Register( const DmMessageNumber number, const DraftMiller::MessageHandler& handler )
{
    // TODO: Log handler shadowing another?
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
        // TODO: Log?
        return DmEncodeMessage( DmMessage::Ptr( new DmFailure ) );
    }
    catch ( DmIncompletePacketException& ex )
    {
        // Fragmented packet, need to continue reading...
        // TODO: Figure out what the continuation mechanism will be here
        throw;
    }
    catch ( Exception& ex )
    {
        // TODO: Log?
        return DmEncodeMessage( DmMessage::Ptr( new DmFailure ) );
    }
}

} // Magenta
