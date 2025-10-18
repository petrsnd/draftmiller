include_directories("${PROJECT_SOURCE_DIR}")

add_library(common
        Exception.h
        Primitives.h
        DataBuffer.cpp
        DataBuffer.h
        DataTypes.cpp
        DataTypes.h
        StringUtils.cpp
        StringUtils.h)

copy_headers_to_include()

add_executable(test-data-buffer TestDataBuffer.cpp UnitTestSuite.h)
target_link_libraries(test-data-buffer common)

add_executable(test-data-types TestDataTypes.cpp UnitTestSuite.h)
target_link_libraries(test-data-types common)

add_executable(test-string-utils TestStringUtils.cpp UnitTestSuite.h)
target_link_libraries(test-string-utils common)

