cmake_minimum_required(VERSION 3.0)

project(http_server)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/source")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

file(GLOB_RECURSE CXX_SRC "${SOURCE_DIR}/*.cpp")

add_executable(http_server ${CXX_SRC})

target_include_directories(http_server PRIVATE ${INCLUDE_DIR})

install(TARGETS http_server RUNTIME DESTINATION bin)
