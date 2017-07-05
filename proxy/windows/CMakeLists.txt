include_directories( "${PROJECT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/include" )
link_directories( "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" )

add_executable( agent
        Main.cpp
        RequestIdentitiesHandler.cpp )

target_link_libraries( agent draftmiller logger common )
