cmake_minimum_required(VERSION 3.25)

project(http_server)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/source")

file(GLOB_RECURSE CXX_SRC "${SOURCE_DIR}/*.cpp")

add_executable(http_server ${CXX_SRC})

target_include_directories(http_server PRIVATE ${CMAKE_SOURCE_DIR}/include)

install(TARGETS http_server RUNTIME DESTINATION bin)
