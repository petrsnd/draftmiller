include_directories( "${PROJECT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/include" )
link_directories( "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" )

set( OSLOGGERS )

if ( "${OPERATINGSYSTEM}" STREQUAL "DARWIN"
        OR "${OPERATINGSYSTEM}" STREQUAL "LINUX" )
    set( OSLOGGERS SyslogLogger.h SyslogLogger.cpp )
endif ()

add_library( logger
        Enum.h
        Logger.h
        ConsoleLogger.h
        ConsoleLogger.cpp
        ${OSLOGGERS} )

copy_headers_to_include()
