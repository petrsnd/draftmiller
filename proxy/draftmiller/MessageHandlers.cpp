// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "DraftMiller.h"

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
    // TODO: Log? -- handler shadowing another?
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
        // TODO: Log? -- unhandled message
        return DmEncodeMessage( DmMessage::Ptr( new DmFailure ) );
    }
    catch ( const DmIncompletePacketException& ex )
    {
        // Fragmented packet, need to continue reading...
        throw;
    }
    catch ( const Exception& ex )
    {
        // TODO: Log? -- unexpected exception log it and return a DmFailure
        return DmEncodeMessage( DmMessage::Ptr( new DmFailure ) );
    }
}

} // Magenta
