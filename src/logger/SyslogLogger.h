#pragma once

// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "Logger.h"

#include <iostream>

namespace DraftMiller {

class SyslogLogger : public LoggerBase
{
public:
    SyslogLogger( const LoggerSeverity& severity, const LoggerDebugLevel& debugLevel, const int facility );
    virtual void Log( const std::string& logMessage );
private:
    int m_facility;
};

void RegisterSyslogLogger(
    const int facility,
    const bool writeToStdErr = false,
    const LoggerDebugLevel& debugThreshold = LoggerDebugLevel::LEVEL_0 );

} // Magenta
