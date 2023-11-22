# message("Path to FileMapper is [${CMAKE_CURRENT_LIST_DIR}]")
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

set(FILEMAPPER_SRCS)

list(APPEND FILEMAPPER_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/src/file_mapper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/include/FileMapper/file_mapper.h
)
