cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(http_server)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/source")
file(GLOB_RECURSE APP_SRC
    "${SOURCE_DIR}/*.cpp"
)

# Extract main.cpp separately
set(MAIN_SRC "${SOURCE_DIR}/main.cpp")

# Remove main from app sources
list(REMOVE_ITEM APP_SRC "${MAIN_SRC}")

add_subdirectory(parsers)
add_subdirectory(test)

add_executable(
    http_server
    ${MAIN_SRC}
    ${APP_SRC}
    ${PARSERS_SRC}
)
add_dependencies(http_server HttpParserGen HttpLexerGen)

target_include_directories(http_server PUBLIC ${CMAKE_SOURCE_DIR}/include ${PARSERS_INCLUDE_DIRS})
