include_directories( "${PROJECT_SOURCE_DIR}" )

add_library( common
	Exception.h
	Primitives.h
	DataBuffer.cpp
	DataBuffer.h
	DataTypes.cpp
	DataTypes.h
	StringUtils.cpp
	StringUtils.h )

copy_headers_to_include()

add_executable( testdatabuffer TestDataBuffer.cpp UnitTestSuite.h )
target_link_libraries( testdatabuffer common )

add_executable( testdatatypes TestDataTypes.cpp UnitTestSuite.h )
target_link_libraries( testdatatypes common )

add_executable( teststringutils TestStringUtils.cpp UnitTestSuite.h )
target_link_libraries( teststringutils common )

