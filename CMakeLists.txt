cmake_minimum_required(VERSION 3.9.6)
project(Babel VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
conan_set_find_paths()


set(CLIENT_DIR ${CMAKE_SOURCE_DIR}/client)
set(CLIENT_UI_DIR ${CLIENT_DIR}/form)
set(CLIENT_SOURCES_DIR ${CLIENT_DIR}/src)
set(CLIENT_SOURCES ${CLIENT_SOURCES_DIR}/main.cpp
		   ${CLIENT_SOURCES_DIR}/MainWindow.cpp
		   ${CLIENT_SOURCES_DIR}/LoginWindow.cpp
		   ${CLIENT_SOURCES_DIR}/CallWindow.cpp
		   ${CLIENT_SOURCES_DIR}/Protocol.cpp
		   ${CLIENT_SOURCES_DIR}/CallReceiveWindow.cpp
		   ${CLIENT_SOURCES_DIR}/TcpClient.cpp
		   ${CLIENT_SOURCES_DIR}/UdpClient.cpp
		   ${CLIENT_SOURCES_DIR}/CodecManager.cpp
		   ${CLIENT_SOURCES_DIR}/SoundManager.cpp)
set(CLIENT_UI	   ${CLIENT_UI_DIR}/CallWindow.ui
                   ${CLIENT_UI_DIR}/CallReceiveWindow.ui
  		   ${CLIENT_UI_DIR}/LoginWindow.ui
		   ${CLIENT_UI_DIR}/MainWindow.ui)

set(SERVER_DIR ${CMAKE_SOURCE_DIR}/server)
set(SERVER_SOURCES_MAIN	   ${SERVER_DIR}/main.cpp)
set(SERVER_SOURCES	   ${SERVER_DIR}/protocol/Protocol.cpp
			   ${SERVER_DIR}/data/Data.cpp
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
						      ${SERVER_DIR}/data
						      ${CONAN_INCLUDE_DIRS})

add_executable(babel_server ${SERVER_SOURCES_MAIN} ${SERVER_SOURCES})
conan_target_link_libraries(babel_server)
target_include_directories(babel_server PRIVATE ${SERVER_DIR}/data
						${SERVER_DIR}/protocol
						${SERVER_DIR}/tcpserver
						${CONAN_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_FLAGS "-fPIC")

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CLIENT_UI_DIR})
find_package(${CONAN_LIB_DIRS_QT}/cmake/Qt5Widgets/Qt5Widgets CONFIG REQUIRED)

add_executable(babel_client ${CLIENT_SOURCES})
conan_target_link_libraries(babel_client)
target_link_libraries(babel_client ${CONAN_LIB_DIRS_QT}/libQt5Gui.so ${CONAN_LIB_DIRS_QT}/libQt5Core.so ${CONAN_LIB_DIRS_QT}/libQt5Widgets.so ${CONAN_LIB_DIRS_QT}/libQt5Network.so ${CONAN_LIB_DIRS_QT}/libQt5XcbQpa.so ${CONAN_LIB_DIRS_QT}/libQt5DBus.so)
target_include_directories(babel_client PRIVATE ${CLIENT_DIR}/include
						${CONAN_INCLUDE_DIRS})

add_custom_command(TARGET babel_client
			COMMAND mkdir -p bin/platforms
			COMMAND ln -sf ${CONAN_QT_ROOT}/plugins/platforms/libqxcb.so bin/platforms/.)
