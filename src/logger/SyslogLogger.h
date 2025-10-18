#pragma once

// (c) 2025 petrsnd@secureidx.com.  All rights reserved.

#include "Logger.h"

namespace DraftMiller {

class SyslogLogger : public LoggerBase
{
public:
    SyslogLogger(const LoggerSeverity& severity, const LoggerDebugLevel& debugLevel, const int facility);
    virtual void Log(const std::string& logMessage);
private:
    int m_facility;
};

void RegisterSyslogLogger(const int facility, const bool writeToStdErr = false,
    const LoggerDebugLevel& debugThreshold = LoggerDebugLevel::LEVEL_0);

} // Magenta
