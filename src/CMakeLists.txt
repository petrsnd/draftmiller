cmake_minimum_required( VERSION 3.8.2 )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/" )
set( CMAKE_CXX_STANDARD 14 )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib 
     CACHE PATH "Output directory for static libraries." )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
     CACHE PATH "Output directory for shared libraries." )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
     CACHE PATH "Output directory for executables and shared libraries." )

file( STRINGS "../copyright.txt" COPYRIGHT_INFO )
file( STRINGS "../version.txt" FULL_VERSION )
file( STRINGS "../build.txt" BUILD_NUMBER )

string( REPLACE "." ";" FULL_VERSION_LIST ${FULL_VERSION} )
list( GET FULL_VERSION_LIST 0 CMAKE_MAJOR_VERSION )
list( GET FULL_VERSION_LIST 1 CMAKE_MINOR_VERSION )
list( GET FULL_VERSION_LIST 2 CMAKE_PATCH_VERSION )

project( Magenta CXX )

add_subdirectory( common )
add_subdirectory( draftmiller )

enable_testing()
add_test( NAME TestDataTypes COMMAND testdatatypes )
add_test( NAME TestDataBuffer COMMAND testdatabuffer )
add_test( NAME TestStringUtils COMMAND teststringutils )

