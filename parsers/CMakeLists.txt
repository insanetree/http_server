find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/http)

bison_target(
    HttpParser
    ${CMAKE_CURRENT_SOURCE_DIR}/http/parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/http/parser.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/http/parser.hpp
)

flex_target(
    HttpLexer
    ${CMAKE_CURRENT_SOURCE_DIR}/http/lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/http/lexer.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/http/lexer.hpp
)

add_flex_bison_dependency(HttpLexer HttpParser)

add_library(parsers OBJECT
    ${BISON_HttpParser_OUTPUTS}
    ${FLEX_HttpLexer_OUTPUTS}
)

target_include_directories(parsers PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/include
)