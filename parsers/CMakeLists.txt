find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

bison_target(
    HttpParser
    ${CMAKE_CURRENT_SOURCE_DIR}/http_parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/http_parser.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/http_parser.hpp
)

flex_target(
    HttpLexer
    ${CMAKE_CURRENT_SOURCE_DIR}/http_lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/http_lexer.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/http_lexer.hpp
)

add_flex_bison_dependency(HttpLexer HttpParser)

set(
    PARSERS_SRC
    ${BISON_HttpParser_OUTPUTS}
    ${FLEX_HttpLexer_OUTPUTS}
    PARENT_SCOPE
)

set(
    PARSERS_INCLUDE_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}
    PARENT_SCOPE
)

add_custom_target(HttpParserGen DEPENDS ${BISON_HttpParser_OUTPUTS})
add_custom_target(HttpLexerGen DEPENDS ${FLEX_HttpLexer_OUTPUTS})
