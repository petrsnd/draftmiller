include_directories("${PROJECT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/include")
link_directories("${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

add_library(draftmiller
        DraftMillerContext.h
        MessageParser.cpp
        MessageEncoder.cpp
        MessageHandlers.cpp)

copy_headers_to_include()

add_executable(test-request-identities TestRequestIdentities.cpp)
target_link_libraries(test-request-identities draftmiller logger common)

add_executable(test-sign-request TestSignRequest.cpp)
target_link_libraries(test-sign-request draftmiller logger common)
