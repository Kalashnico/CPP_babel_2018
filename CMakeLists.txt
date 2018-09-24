cmake_minimum_required(VERSION 3.0.0)
project(Babel VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
conan_set_find_paths()

set(SERVER_DIR ${CMAKE_SOURCE_DIR}/server)
set(SERVER_SOURCES_MAIN	   ${SERVER_DIR}/main.cpp)
set(SERVER_SOURCES	   ${SERVER_DIR}/protocol/Protocol.cpp
			   ${SERVER_DIR}/data/Data.cpp
			   ${SERVER_DIR}/tcpserver/ATcpServer.cpp
			   ${SERVER_DIR}/tcpserver/BoostTcpServer.cpp
			   ${SERVER_DIR}/tcpserver/BoostTcpSession.cpp)

set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(SERVER_TEST_DIR ${TEST_DIR}/server)
set(SERVER_TEST_SOURCES ${SERVER_TEST_DIR}/Tests.cpp
			${SERVER_TEST_DIR}/ProtocolTests.cpp
			${SERVER_TEST_DIR}/DataTests.cpp)

add_executable(babel_server_tests ${SERVER_SOURCES} ${SERVER_TEST_SOURCES})
conan_target_link_libraries(babel_server_tests)
target_include_directories(babel_server_tests PRIVATE ${TEST_DIR}/catch2
						      ${SERVER_TEST_DIR}
						      ${SERVER_DIR}/protocol
						      ${SERVER_DIR}/data)

add_executable(babel_server ${SERVER_SOURCES_MAIN} ${SERVER_SOURCES})
conan_target_link_libraries(babel_server)
target_include_directories(babel_server PRIVATE ${SERVER_DIR}/data
						${SERVER_DIR}/protocol
						${SERVER_DIR}/tcpserver)
