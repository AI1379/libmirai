project(UnitTests)

add_subdirectory(./gtest)
include_directories(${gtest_SOURCE_DIR}/include)

include_directories(${MIRAI_INCLUDE_DIR})

set(MIRAI_TESTS_FILES
        ${MIRAI_TESTS_DIR}/utils/encode-test.cpp
        ${MIRAI_TESTS_DIR}/crypto/tea-test.cpp
        ${MIRAI_TESTS_DIR}/crypto/hash-test.cpp
        ${MIRAI_TESTS_DIR}/crypto/ecdh-test.cpp
        ${MIRAI_TESTS_DIR}/protocol/tlv-test.cpp
        ${MIRAI_TESTS_DIR}/protocol/jce-test.cpp
        ${MIRAI_TESTS_DIR}/utils/bytearray-test.cpp
        ${MIRAI_TESTS_DIR}/core/device-test.cpp
        ${MIRAI_TESTS_DIR}/core/network-test.cpp utils/signal-slot-test.cpp utils/string-format-test.cpp)

set(MIRAI_TESTS_FILES ${MIRAI_TESTS_FILES} PARENT_SCOPE)

message("TESTS FILES: ${MIRAI_TESTS_FILES}")
message("SOURCE FILES: ${MIRAI_SOURCE_FILES}")

add_executable(UnitTestsRun ${MIRAI_TESTS_FILES})

target_link_libraries(UnitTestsRun gtest gtest_main mirai)