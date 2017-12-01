// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "SyslogLogger.h"

#include <cstdlib>
#include <syslog.h>

namespace DraftMiller {

static int ConvertToSyslogSeverity( const LoggerSeverity& severity )
{
    switch ( severity )
    {
        case LoggerSeverity::EMERGENCY:
            return LOG_EMERG;
        case LoggerSeverity::ALERT:
            return LOG_ALERT;
        case LoggerSeverity::CRITICAL:
            return LOG_CRIT;
        case LoggerSeverity::ERROR:
            return LOG_ERR;
        case LoggerSeverity::WARNING:
            return LOG_WARNING;
        case LoggerSeverity::NOTICE:
            return LOG_NOTICE;
        case LoggerSeverity::INFO:
            return LOG_INFO;
        case LoggerSeverity::DEBUG:
        default:
            return LOG_DEBUG;
    }
}


SyslogLogger::SyslogLogger( const LoggerSeverity& severity, const LoggerDebugLevel& debugLevel, const int facility ) :
    LoggerBase( severity, debugLevel ),
    m_facility( facility )
{}
void SyslogLogger::Log( const std::string& logMessage )
{
    syslog( m_facility | ConvertToSyslogSeverity( m_severity ), "%s", logMessage.c_str() );
}

void RegisterSyslogLogger(
    const int facility,
    const bool writeToStdErr,
    const LoggerDebugLevel& debugThreshold )
{
    std::atexit( []() {
        closelog();
    } );

    openlog( nullptr, LOG_PID | ( writeToStdErr ? 0 : LOG_PERROR ), facility );

    LoggerConfig::Instance()->Initialize(
        [facility] ( const LoggerSeverity severity, const LoggerDebugLevel debugLevel ) -> LoggerBase::Ptr
        {
            return std::make_shared< SyslogLogger >( severity, debugLevel, facility );
        }, debugThreshold );
}

} // Magenta
