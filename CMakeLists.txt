cmake_minimum_required(VERSION 3.22)
project(mirai)

set(CMAKE_CXX_STANDARD 20)

# === Set compile variables ===
set(EXTERN_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/3rdparty)

set(MIRAI_PROTOBUF_DIR ${CMAKE_SOURCE_DIR}/src/pb)
set(MIRAI_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(MIRAI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(MIRAI_TESTS_DIR ${CMAKE_SOURCE_DIR}/tests)

message("Source Dir: ${MIRAI_SOURCE_DIR}")
message("Include Dir: ${MIRAI_INCLUDE_DIR}")
message("Tests Dir: ${MIRAI_TESTS_DIR}")
message("Protobuf Dir: ${MIRAI_PROTOBUF_DIR}")

# === Add Source Dir ===
add_subdirectory(src)

# === Add Unit Test ===
add_subdirectory(tests)
