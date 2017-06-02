function( copy_headers_to_include )
    make_directory( ${CMAKE_BINARY_DIR}/include )
    file( GLOB HEADERS_TO_COPY
        "./*.h" )
    file( COPY ${HEADERS_TO_COPY} DESTINATION ${CMAKE_BINARY_DIR}/include )
endfunction( copy_headers_to_include )

