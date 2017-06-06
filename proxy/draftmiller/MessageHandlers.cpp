// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "DraftMiller.h"

namespace Magenta {

DraftMiller::Ptr DraftMiller::Instance()
{
    static DraftMiller::Ptr instance = std::make_shared< DraftMiller >();
    return instance;
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
            // TODO: Call encoders
            return DmEncodeFailure();
        }
        return DmEncodeFailure();
    }
    catch ( DmIncompletePacketException& ex )
    {
        throw;
    }
    catch ( Exception& ex )
    {
        // TODO: Log?
        return DmEncodeFailure();
    }
}

} // Magenta
