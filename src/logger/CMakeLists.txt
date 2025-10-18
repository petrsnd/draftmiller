include_directories("${PROJECT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/include")
link_directories("${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

set(OS_LOGGERS)

if ("${OPERATING_SYSTEM}" STREQUAL "DARWIN" OR "${OPERATING_SYSTEM}" STREQUAL "LINUX")
    set(OS_LOGGERS SyslogLogger.h SyslogLogger.cpp)
endif ()

add_library(logger
        Enum.h
        Logger.h
        ConsoleLogger.h
        ConsoleLogger.cpp
        ${OS_LOGGERS})

copy_headers_to_include()
