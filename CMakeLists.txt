cmake_minimum_required(VERSION 3.22)
project(mirai)

set(CMAKE_CXX_STANDARD 17)

# === Set compile variables ===
set(EXTERN_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/3rdparty)

set(MIRAI_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(MIRAI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(MIRAI_TESTS_DIR ${CMAKE_SOURCE_DIR}/tests)

find_package(Threads REQUIRED)

# === import ASIO Standalone version ===
add_library(asio INTERFACE)
set(ASIO_SOURCE_DIR ${EXTERN_LIBRARY_DIR}/asio)
target_include_directories(asio INTERFACE ${ASIO_SOURCE_DIR}/asio/include)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE ASIO_NO_DEPRECATED)
target_link_libraries(asio INTERFACE Threads::Threads)

if(WIN32)
    # macro see @ https://stackoverflow.com/a/40217291/1746503
    macro(get_win32_winnt version)
        if (CMAKE_SYSTEM_VERSION)
            set(ver ${CMAKE_SYSTEM_VERSION})
            string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
            string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
            if ("${verMajor}" MATCHES "10")
                set(verMajor "A")
                string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
            endif ("${verMajor}" MATCHES "10")
            string(REPLACE "." "" ver ${ver})
            string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
            set(${version} "0x${ver}")
        endif()
    endmacro()

    if(NOT DEFINED _WIN32_WINNT)
        get_win32_winnt(ver)
        set(_WIN32_WINNT ${ver})
    endif()

    message(STATUS "Set _WIN32_WINNET=${_WIN32_WINNT}")

    target_compile_definitions(asio
            INTERFACE
            _WIN32_WINNT=${_WIN32_WINNT}
            WIN32_LEAN_AND_MEAN
            )
endif()

include_directories(${ASIO_SOURCE_DIR}/asio/include)

# === Add OpenSSL ===
find_package(OpenSSL REQUIRED)

# === Add Source Dir ===
add_subdirectory(src)

# === Add Unit Test ===
add_subdirectory(tests)

target_link_libraries(mirai asio)
