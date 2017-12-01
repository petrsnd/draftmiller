#pragma once

// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "Enum.h"

#include <functional>
#include <memory>
#include <string>
#include <sstream>

namespace DraftMiller {

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
    LoggerBase( const LoggerSeverity& severity, const LoggerDebugLevel& debugLevel ) :
        m_severity( severity ),
        m_debugLevel( debugLevel )
    {}
    virtual ~LoggerBase()
    {}
    virtual void Log( const std::string& logMessage ) = 0;
    bool ShouldLog( const LoggerDebugLevel& debugThreshold )
    {
        if ( m_severity._to_integral() == LoggerSeverity::DEBUG
             && m_debugLevel._to_integral() > debugThreshold )
        {
            return false;
        }
        return true;
    }
protected:
    LoggerSeverity m_severity;
    LoggerDebugLevel m_debugLevel;
};

// Lazy logger creation
typedef std::function< LoggerBase::Ptr( const LoggerSeverity&, const LoggerDebugLevel& ) > LoggerCreator;


class LoggerImpl
{
public:
    LoggerImpl( const LoggerBase::Ptr& logger, const LoggerDebugLevel& debugThreshold ) :
        m_logger( logger ),
        m_debugThreshold( debugThreshold )
    {}
    LoggerImpl( const LoggerImpl& loggerImpl ) :
        m_logger( loggerImpl.m_logger ),
        m_debugThreshold( LoggerDebugLevel::LEVEL_0 )
    {
        m_ss << loggerImpl.m_ss.str();
    }
    ~LoggerImpl()
    {
        if ( m_logger->ShouldLog( m_debugThreshold ) )
        {
            m_logger->Log( m_ss.str() );
        }
    }
    template< typename T >
    LoggerImpl& operator<<(T value)
    {
        m_ss << value;
        return *this;
    }
private:
    LoggerBase::Ptr m_logger;
    LoggerDebugLevel m_debugThreshold;
    std::stringstream m_ss;
};

class NullLogger : public LoggerBase
{
public:
    NullLogger( const LoggerSeverity& severity, const LoggerDebugLevel& debugLevel ) :
        LoggerBase( severity, debugLevel )
    {}
    virtual void Log( const std::string& logMessage )
    {}
};

class LoggerConfig
{
public:
    typedef std::shared_ptr< LoggerConfig > Ptr;
private:
    LoggerConfig() :
        m_debugThreshold( LoggerDebugLevel::LEVEL_0 )
    {
        // default to null logger
        m_loggerCreator = [] ( const LoggerSeverity& severity, const LoggerDebugLevel& debugLevel )
        {
            return std::make_shared< NullLogger >( severity, debugLevel );
        };
    }
public:
    static LoggerConfig::Ptr Instance()
    {
        static LoggerConfig::Ptr instance = LoggerConfig::Ptr( new LoggerConfig );
        return instance;
    }
    void Initialize( const LoggerCreator& loggerCreator )
    {
        m_loggerCreator = loggerCreator;
        m_debugThreshold = LoggerDebugLevel::LEVEL_0;
    }
    void Initialize( const LoggerCreator& loggerCreator, const LoggerDebugLevel& debugThreshold )
    {
        m_loggerCreator = loggerCreator;
        m_debugThreshold = debugThreshold;
    }
    LoggerImpl CreateLogger(
        const LoggerSeverity& severity, const LoggerDebugLevel& debugLevel = LoggerDebugLevel::LEVEL_0 )
    {
        return LoggerImpl( m_loggerCreator( severity, debugLevel ), m_debugThreshold );
    }
private:
    LoggerCreator m_loggerCreator;
    LoggerDebugLevel m_debugThreshold;
};


// Interface
#define LOG_EMERG   LoggerConfig::Instance()->CreateLogger( LoggerSeverity::EMERGENCY )
#define LOG_ALERT   LoggerConfig::Instance()->CreateLogger( LoggerSeverity::ALERT )
#define LOG_CRIT    LoggerConfig::Instance()->CreateLogger( LoggerSeverity::CRITICAL )
#define LOG_ERR     LoggerConfig::Instance()->CreateLogger( LoggerSeverity::ERROR )
#define LOG_WARN    LoggerConfig::Instance()->CreateLogger( LoggerSeverity::WARNING )
#define LOG_NOTICE  LoggerConfig::Instance()->CreateLogger( LoggerSeverity::NOTICE )
#define LOG_INFO    LoggerConfig::Instance()->CreateLogger( LoggerSeverity::INFO )
#define LOG_DBG1    LoggerConfig::Instance()->CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_1 )
#define LOG_DBG2    LoggerConfig::Instance()->CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_2 )
#define LOG_DBG3    LoggerConfig::Instance()->CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_3 )
#define LOG_DBG4    LoggerConfig::Instance()->CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_4 )
#define LOG_DBG5    LoggerConfig::Instance()->CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_5 )
#define LOG_DBG6    LoggerConfig::Instance()->CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_6 )
#define LOG_DBG7    LoggerConfig::Instance()->CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_7 )
#define LOG_DBG8    LoggerConfig::Instance()->CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_8 )
#define LOG_DBG9    LoggerConfig::Instance()->CreateLogger( LoggerSeverity::DEBUG, LoggerDebugLevel::LEVEL_9 )

} // Magenta
