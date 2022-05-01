include_directories(${MIRAI_INCLUDE_DIR})

set(MIRAI_SOURCE_FILES
        ${MIRAI_SOURCE_DIR}/utils/encode.cpp)

set(MIRAI_SOURCE_FILES ${MIRAI_SOURCE_FILES} PARENT_SCOPE)

add_library(mirai ${MIRAI_SOURCE_FILES})