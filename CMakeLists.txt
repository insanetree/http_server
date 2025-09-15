cmake_minimum_required(VERSION 3.25)

project(http_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/source")
file(GLOB_RECURSE CXX_SRC "${SOURCE_DIR}/*.cpp")

add_subdirectory(parsers)

add_executable(http_server ${CXX_SRC})

target_link_libraries(http_server PUBLIC parsers)
target_include_directories(http_server PUBLIC ${CMAKE_SOURCE_DIR}/include)
