include_directories( "${PROJECT_SOURCE_DIR}" )

add_library( draftmiller
        DraftMiller.h
        MessageParser.cpp)

copy_headers_to_include()