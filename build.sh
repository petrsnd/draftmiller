#!/bin/bash
PREVIOUS_DIR=`pwd`
SCRIPT_DIR=`cd \`dirname $0\` && pwd`
BUILD_ARGS=
CLEAN=
FORCE=
PACKAGE=
CONF="Release"
TOOL="$0"
VERSION=
BUILDNUMBER=
FULL_VERSION=

cd "$SCRIPT_DIR"
return_to_previous_directory()
{
    cd "$PREVIOUS_DIR"
}
trap return_to_previous_directory INT TERM EXIT


check_for_tool_in_path()
{
    if [ -z "`which $1 2>/dev/null`" ]; then
        echo "Unable to find $1 in $PATH"
        echo "Consider getting it from homebrew or apt?"
        exit 1
    fi
}
check_cmake_version()
{
    CMAKE_VERSION=`cmake --version | head -1 | cut -f3 -d' '`
    echo "CMake $CMAKE_VERSION found"
    CMAKE_MAJOR=`echo $CMAKE_VERSION | cut -f1 -d.`
    CMAKE_MINOR=`echo $CMAKE_VERSION | cut -f2 -d.`
    CMAKE_MICRO=`echo $CMAKE_VERSION | cut -f3 -d.`
    if [ $CMAKE_MAJOR -lt 3 -o $CMAKE_MINOR -lt 1 ]; then
        echo "CMake version 3.8.2 or greater is required"
        exit 1
    fi
}
check_for_prerequisites()
{
    check_for_tool_in_path cmake
    check_for_tool_in_path dos2unix
    check_cmake_version
}
set_version()
{
    echo "Setting version information for build"
    cp $SCRIPT_DIR/version.txt /tmp/version.txt
    dos2unix /tmp/version.txt
    VERSION=`cat /tmp/version.txt`
    cp $SCRIPT_DIR/build.txt /tmp/build.txt
    dos2unix /tmp/build.txt
    BUILDNUMBER=`cat /tmp/build.txt`
    FULL_VERSION=`echo $VERSION.$BUILDNUMBER`
    echo "FULL_VERSION: $FULL_VERSION"
}
configure()
{
    if [ ! -f "$SCRIPT_DIR/Build/CMakeCache.txt" ]; then
        set -e
        rm -rf "$SCRIPT_DIR/Build"
        mkdir -p "$SCRIPT_DIR/Build"
        cd "$SCRIPT_DIR/Build"
        cmake ../ -G "Unix Makefiles"
        set +e
    fi
}
build()
{
    set -e
    cd "$SCRIPT_DIR/Build"
    make
    if [ $? -eq 0 ]; then
        echo "Build succeeded -- Artifacts at $SCRIPT_DIR/Build/"
    else
        echo "Build failed!"
        exit 1
    fi
    make test
    if [ $? -eq 0 ]; then
        echo "Unit tests succeeded"
    else
        echo "Unit tests failed!"
        exit 1
    fi
    if [ "$PACKAGE" = "true" ]; then
        echo "TODO: Code Signing ..."
    fi
    cd "$SCRIPT_DIR"
    set +e
}
package()
{
    set -e
    echo "TODO: Packaging ..."
    set +e
}
print_usage()
{
    echo "USAGE: $TOOL [-c] [-d] [-f] [-p] [-h]"
    echo "Build Magenta using CMake and Makefiles"
    echo "  -c  Just clean up current build files"
    echo "  -d  Build with debug configuration"
    echo "  -f  Force a reconfigure and a rebuild"
    echo "  -h  Print this message and exit"
    echo "  -p  Build the package after successful build"
    exit 1
}

# Main Script
argv=`getopt cdfhp "$@" 2>/dev/null`
if [ $? -ne 0 ]; then 
    echo "Invalid option(s)!"
    print_usage
fi 
set -- $argv
while [ $# -gt 0 ]; do
    case $1 in
        -c ) CLEAN="true"; shift ;;
        -f ) FORCE="true"; shift ;;
        -d ) CONF="Debug"; shift ;;
        -h ) HELP="true"; shift ;;
        -p ) PACKAGE="true"; shift ;;
        -- ) shift; break ;;
        *  ) echo "Invalid option(s)!"; print_usage; ;;
    esac 
done 
if [ "$HELP" = "true" ]; then
    print_usage
fi
if [ "$FORCE" = "true" -o "$CLEAN" = "true" ]; then
    rm -rf "$SCRIPT_DIR/Build"
    echo "Removed files at $SCRIPT_DIR/Build"
    if [ "$CLEAN" = "true" ]; then exit 0; fi
fi

check_for_prerequisites
set_version
configure
build
if [ "$PACKAGE" = "true" ]; then
    package
fi

