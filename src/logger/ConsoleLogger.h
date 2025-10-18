#pragma once

// (c) 2025 petrsnd@secureidx.com.  All rights reserved.

#include "Logger.h"

namespace DraftMiller {

class ConsoleLogger : public LoggerBase
{
public:
    ConsoleLogger(const LoggerSeverity& severity, const LoggerDebugLevel& debugLevel);
    void Log(const std::string& logMessage) override;
};

void RegisterConsoleLogger(const LoggerDebugLevel& debugThreshold = LoggerDebugLevel::LEVEL_0);

} // Magenta
