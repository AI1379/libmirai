project(UnitTests)

add_subdirectory(./gtest)
include_directories(${gtest_SOURCE_DIR}/include)

include_directories(${MIRAI_INCLUDE_DIR})

set(MIRAI_TESTS_FILES
        ${MIRAI_TESTS_DIR}/utils/encode-test.cpp
        ${MIRAI_TESTS_DIR}/crypto/tea-test.cpp
        )

set(MIRAI_TESTS_FILES ${MIRAI_TESTS_FILES} PARENT_SCOPE)

message("TESTS FILES: ${MIRAI_TESTS_FILES}")
message("SOURCE FILES: ${MIRAI_SOURCE_FILES}")

add_executable(UnitTestsRun ${MIRAI_TESTS_FILES})

add_library(MiraiLib ${MIRAI_SOURCE_FILES})

target_link_libraries(UnitTestsRun MiraiLib gtest gtest_main)