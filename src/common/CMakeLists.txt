INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}" )

ADD_LIBRARY( common
	BaseException.h
	Primitives.h
	DataBuffer.cpp
	DataBuffer.h
	DataTypes.cpp
	DataTypes.h
	StringUtils.cpp
	StringUtils.h )

ADD_EXECUTABLE( testdatabuffer TestDataBuffer.cpp UnitTestSuite.h )
TARGET_LINK_LIBRARIES( testdatabuffer essentials )

ADD_EXECUTABLE( testdatatypes TestDataTypes.cpp UnitTestSuite.h )
TARGET_LINK_LIBRARIES( testdatatypes essentials )

ADD_EXECUTABLE( teststringutils TestStringUtils.cpp UnitTestSuite.h )
TARGET_LINK_LIBRARIES( teststringutils essentials )

ENABLE_TESTING()
ADD_TEST( DataBuffer testdatabuffer )
ADD_TEST( DataTypes testdatatypes )
ADD_TEST( StringUtil teststringutils )

