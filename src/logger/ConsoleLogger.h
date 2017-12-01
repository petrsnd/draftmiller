#pragma once

// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "Logger.h"

#include <iostream>

namespace DraftMiller {

class ConsoleLogger : public LoggerBase
{
public:
    ConsoleLogger( const LoggerSeverity& severity, const LoggerDebugLevel& debugLevel );
    virtual void Log( const std::string& logMessage );
};

void RegisterConsoleLogger( const LoggerDebugLevel& debugThreshold = LoggerDebugLevel::LEVEL_0 );

} // Magenta
