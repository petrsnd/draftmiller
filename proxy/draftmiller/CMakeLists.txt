include_directories( "${PROJECT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/include" )
link_directories( "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" )

add_library( draftmiller
        DraftMiller.h
        MessageParser.cpp
        MessageEncoder.cpp
        MessageHandlers.cpp )

copy_headers_to_include()

add_executable( testrequestidentities TestRequestIdentities.cpp )
target_link_libraries( testrequestidentities draftmiller common )
