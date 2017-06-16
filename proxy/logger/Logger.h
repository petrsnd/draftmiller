#pragma once

// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "Enum.h"

#include <functional>
#include <memory>
#include <string>
#include <sstream>

namespace Magenta {

BETTER_ENUM( LoggerSeverity, char,
    EMERGENCY = 0,
    ALERT = 1,
    CRITICAL = 2,
    ERROR = 3,
    WARNING = 4,
    NOTICE = 5,
    INFO = 6,
    DEBUG = 7
)

BETTER_ENUM( LoggerDebugLevel, char,
    LEVEL_0 = 0,
    LEVEL_1 = 1,
    LEVEL_2 = 2,
    LEVEL_3 = 3,
    LEVEL_4 = 4,
    LEVEL_5 = 5,
    LEVEL_6 = 6,
    LEVEL_7 = 7,
    LEVEL_8 = 8,
    LEVEL_9 = 9
)

class LoggerBase
{
public:
    typedef std::shared_ptr< LoggerBase > Ptr;
    LoggerBase( const LoggerSeverity severity, const LoggerDebugLevel debugLevel ) :
        m_severity( severity ),
        m_debugLevel( debugLevel )
    {}
    virtual ~LoggerBase()
    {}
    virtual void Log( const std::string& logMessage ) = 0;
protected:
    LoggerSeverity m_severity;
    LoggerDebugLevel m_debugLevel;
};

// Initialization and logger creation
typedef std::function< void() > LoggerInit;
typedef std::function< LoggerBase::Ptr( const LoggerSeverity, const LoggerDebugLevel ) > LoggerCreator;


class LoggerImpl
{
public:
    LoggerImpl( const LoggerBase::Ptr& logger ) :
        m_logger( logger )
    {}
    LoggerImpl( const LoggerImpl& loggerImpl ) :
        m_logger( loggerImpl.m_logger )
    {
        m_ss << loggerImpl.m_ss.str();
    }
    ~LoggerImpl()
    {
        m_logger->Log( m_ss.str() );
    }
    template< typename T >
    LoggerImpl& operator<<(T value)
    {
        m_ss << value;
        return *this;
    }
private:
    LoggerBase::Ptr m_logger;
    std::stringstream m_ss;
};

class NullLogger : public LoggerBase
{
public:
    NullLogger( const LoggerSeverity severity, const LoggerDebugLevel debugLevel ) :
        LoggerBase( severity, debugLevel )
    {}
    virtual void Log( const std::string& logMessage )
    {}
};

class LoggerConfig
{
public:
    static void Initialize( const LoggerInit& loggerInit, const LoggerCreator& loggerCreator )
    {
        loggerInit();
        s_loggerCreator = loggerCreator;
    }
    static LoggerImpl CreateLogger(
        const LoggerSeverity severity, const LoggerDebugLevel debugLevel = LoggerDebugLevel::LEVEL_0)
    {
        return LoggerImpl( s_loggerCreator( severity, debugLevel ) );
    }
private:
    static LoggerCreator s_loggerCreator;
    friend class DefaultInitializer;
    struct DefaultInitializer
    {
        DefaultInitializer()
        {
            s_loggerCreator = [] ( const LoggerSeverity severity, const LoggerDebugLevel debugLevel )
            {
                return std::make_shared< NullLogger >( severity, debugLevel );
            };
        }
    };
    static DefaultInitializer s_defaultInitializer;
};


// Interface
#define LOG_EMERG   LoggerConfig::CreateLogger( LoggerSeverity::EMERGENCY )
#define LOG_ALERT   LoggerConfig::CreateLogger( LoggerSeverity::ALERT )
#define LOG_CRIT    LoggerConfig::CreateLogger( LoggerSeverity::CRITICAL )
#define LOG_ERR     LoggerConfig::CreateLogger( LoggerSeverity::ERROR )
#define LOG_WARN    LoggerConfig::CreateLogger( LoggerSeverity::WARNING )
#define LOG_NOTICE  LoggerConfig::CreateLogger( LoggerSeverity::NOTICE )
#define LOG_INFO    LoggerConfig::CreateLogger( LoggerSeverity::INFO )
#define LOG_DBG1    LoggerConfig::CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_1 )
#define LOG_DBG2    LoggerConfig::CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_2 )
#define LOG_DBG3    LoggerConfig::CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_3 )
#define LOG_DBG4    LoggerConfig::CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_4 )
#define LOG_DBG5    LoggerConfig::CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_5 )
#define LOG_DBG6    LoggerConfig::CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_6 )
#define LOG_DBG7    LoggerConfig::CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_7 )
#define LOG_DBG8    LoggerConfig::CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_8 )
#define LOG_DBG9    LoggerConfig::CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_9 )

} // Magenta
