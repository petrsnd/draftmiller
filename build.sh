#!/bin/bash
PREVDIR=`pwd`
SCRIPTDIR=`cd \`dirname $0\` && pwd`
BUILDARGS=
CLEAN=
FORCE=
PACKAGE=
CONF="Release"
TOOL="$0"
VERSION=
BUILDNUMBER=
FULLVERSION=

cd "$SCRIPTDIR"
return_to_previous_directory()
{
    cd "$PREVDIR"
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
    CMAKEVER=`cmake --version | head -1 | cut -f3 -d' '`
    echo "CMake $CMAKEVER found"
    CMAKEMAJOR=`echo $CMAKEVER | cut -f1 -d.`
    CMAKEMINOR=`echo $CMAKEVER | cut -f2 -d.`
    CMAKEMICRO=`echo $CMAKEVER | cut -f3 -d.`
    if [ $CMAKEMAJOR -lt 3 -o $CMAKEMINOR -lt 1 ]; then
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
    cp $SCRIPTDIR/version.txt /tmp/version.txt
    dos2unix /tmp/version.txt
    VERSION=`cat /tmp/version.txt`
    cp $SCRIPTDIR/build.txt /tmp/build.txt
    dos2unix /tmp/build.txt
    BUILDNUMBER=`cat /tmp/build.txt`
    FULLVERSION=`echo $VERSION.$BUILDNUMBER`
    echo "FULLVERSION: $FULLVERSION"
}
configure()
{
    if [ ! -f "$SCRIPTDIR/Build/CMakeCache.txt" ]; then
        set -e
        rm -rf "$SCRIPTDIR/Build"
        mkdir -p "$SCRIPTDIR/Build"
        cd "$SCRIPTDIR/Build"
        cmake ../src -G "Unix Makefiles"
        set +e
    fi
}
build()
{
    set -e
    cd "$SCRIPTDIR/Build"
    make
    if [ -e "$BUNDLE" ]; then
        echo "Build success -- Artifacts at $SCRIPTDIR/Build"
    else
        echo "Build failed!"
        exit 1
    fi
    if [ "$PACKAGE" = "true" ]; then
        echo "TODO: Code Signing ..."
    fi
    cd "$SCRIPTDIR"
    set +e
}
package()
{
    set -e
    echo "TODO:"
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
    rm -rf "$SCRIPTDIR/Build"
    echo "Removed files at $SCRIPTDIR/Build"
    if [ "$CLEAN" = "true" ]; then exit 0; fi
fi

check_for_prerequisites
set_version
configure
build
if [ "$PACKAGE" = "true" ]; then
    package
fi

