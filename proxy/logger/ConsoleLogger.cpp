// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "ConsoleLogger.h"

namespace Magenta {

ConsoleLogger::ConsoleLogger( const LoggerSeverity severity, const LoggerDebugLevel debugLevel ) :
    LoggerBase( severity, debugLevel )
{}
void ConsoleLogger::Log( const std::string& logMessage )
{
    std::cout << "[" << m_severity._to_string()
              << ( m_debugLevel._to_integral() != LoggerDebugLevel::LEVEL_0 ?
                   ":" + std::string( m_debugLevel._to_string() ) + "]" : "]" )
              << logMessage << std::endl;
}

void RegisterConsoleLogger()
{
    LoggerConfig::Instance()->Initialize(
        [] ( const LoggerSeverity severity, const LoggerDebugLevel debugLevel ) -> LoggerBase::Ptr
        {
            return std::make_shared< ConsoleLogger >( severity, debugLevel );
        } );
}

} // Magenta
