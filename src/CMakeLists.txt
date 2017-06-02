cmake_minimum_required( VERSION 3.8.2 )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/" )
set( CMAKE_CXX_STANDARD 14 )

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

