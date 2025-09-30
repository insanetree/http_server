include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(
    googletest
)

enable_testing()

file(GLOB_RECURSE TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(unit_test
    ${TEST_SRC}
    ${APP_SRC}         # reuse main sources
    ${PARSERS_SRC}     # reuse parser
)

target_include_directories(unit_test PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${PARSERS_INCLUDE_DIRS}
)

target_link_libraries(unit_test
    GTest::gtest_main
)

add_dependencies(unit_test HttpParserGen HttpLexerGen)

include(GoogleTest)
gtest_discover_tests(unit_test)