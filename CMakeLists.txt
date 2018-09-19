cmake_minimum_required(VERSION 3.0.0)
project(Babel VERSION 0.1.0)
add_definitions("-std=c++17")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(SERVER_DIR ${CMAKE_SOURCE_DIR}/server)
set(SERVER_SOURCES ${SERVER_DIR}/protocol/Protocol.cpp)

set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(SERVER_TEST_DIR ${TEST_DIR}/server)
set(SERVER_TEST_SOURCES ${SERVER_TEST_DIR}/ProtocolTests.cpp)

add_executable(babel_server_tests ${SERVER_SOURCES} ${SERVER_TEST_SOURCES})
target_include_directories(babel_server_tests PUBLIC ${TEST_DIR}/catch2)
target_include_directories(babel_server_tests PRIVATE ${SERVER_DIR}/protocol)
