cmake_minimum_required(VERSION 3.0)

project(http_server)

add_executable(http_server main.cpp)

install(TARGETS http_server RUNTIME DESTINATION bin)
