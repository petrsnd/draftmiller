INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}" )

ADD_LIBRARY( common
	Exception.h
	Primitives.h
	DataBuffer.cpp
	DataBuffer.h
	DataTypes.cpp
	DataTypes.h
	StringUtils.cpp
	StringUtils.h )

ADD_EXECUTABLE( testdatabuffer TestDataBuffer.cpp UnitTestSuite.h )
TARGET_LINK_LIBRARIES( testdatabuffer common )

ADD_EXECUTABLE( testdatatypes TestDataTypes.cpp UnitTestSuite.h )
TARGET_LINK_LIBRARIES( testdatatypes common )

ADD_EXECUTABLE( teststringutils TestStringUtils.cpp UnitTestSuite.h )
TARGET_LINK_LIBRARIES( teststringutils common )

ENABLE_TESTING()
ADD_TEST( DataBuffer testdatabuffer )
ADD_TEST( DataTypes testdatatypes )
ADD_TEST( StringUtil teststringutils )

