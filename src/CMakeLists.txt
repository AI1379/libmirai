include_directories(${MIRAI_INCLUDE_DIR})

set(MIRAI_SOURCE_FILES
        ${MIRAI_SOURCE_DIR}/utils/encode.cpp
        ${MIRAI_SOURCE_DIR}/protocol/tlv.cpp
        ${MIRAI_SOURCE_DIR}/crypto/ecdh.cpp
        ${MIRAI_SOURCE_DIR}/protocol/jce.cpp
        ${MIRAI_SOURCE_DIR}/utils/bytearray.cpp)

set(MIRAI_SOURCE_FILES ${MIRAI_SOURCE_FILES} PARENT_SCOPE)

find_package(Threads REQUIRED)

# === import ASIO Standalone version ===
add_library(asio INTERFACE)
set(ASIO_SOURCE_DIR ${EXTERN_LIBRARY_DIR}/asio)
target_include_directories(asio INTERFACE ${ASIO_SOURCE_DIR}/asio/include)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE ASIO_NO_DEPRECATED)
target_link_libraries(asio INTERFACE Threads::Threads)

if (WIN32)
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
        endif ()
    endmacro()

    if (NOT DEFINED _WIN32_WINNT)
        get_win32_winnt(ver)
        set(_WIN32_WINNT ${ver})
    endif ()

    message(STATUS "Set _WIN32_WINNET=${_WIN32_WINNT}")

    target_compile_definitions(asio
            INTERFACE
            _WIN32_WINNT=${_WIN32_WINNT}
            WIN32_LEAN_AND_MEAN
            )
endif ()

include_directories(${ASIO_SOURCE_DIR}/asio/include)

# === Add OpenSSL ===
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

add_library(mirai ${MIRAI_SOURCE_FILES})

target_link_libraries(mirai asio OpenSSL::Crypto)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(mirai ws2_32)
endif ()