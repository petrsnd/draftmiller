include_directories( "${PROJECT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/include" )
link_directories( "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" )

add_library( logger
        Logger.h
        ConsoleLogger.h
        ConsoleLogger.cpp
        SyslogLogger.h
        SyslogLogger.cpp Enum.h)

copy_headers_to_include()

