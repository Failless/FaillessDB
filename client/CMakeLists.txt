cmake_minimum_required(VERSION 3.15)
project(client)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost ${BOOST_VERSION} COMPONENTS serialization filesystem thread system REQUIRED)
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/llssclient")

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(GMock_INCLUDE_DIR "/usr/local/include")
set(GMock_LIBRARY "/usr/local/lib/libgmock.a")
set(GMock_LIBRARY_MAIN "/usr/local/lib/libgmock_main.a")
set(GMock_BOTH_LIBRARIES ${GMock_LIBRARY} ${GMock_LIBRARY_MAIN})

include_directories(${SRC_DIR})
include_directories(${COMMON_SRC_DIR})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMock_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -std=c++17 -fPIC -no-pie -phtread -lpthread -lboost_system")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -std=c++17 -fPIC -no-pie -pthread -lpthread -lboost_system")
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/llssclient/)

file(GLOB ALL_SOURCES ${SRC_DIR}/core/*.cpp
                      ${SRC_DIR}/network/*.cpp
                      ${SRC_DIR}/filesystem/*.cpp)

file(GLOB ALL_HEADERS ${SRC_DIR}/core/*.h
                      ${SRC_DIR}/network/*.h
                      ${SRC_DIR}/filesystem/*.h)

file(GLOB ALL_TEST_SOURCES ${SRC_DIR}/tests/test_client.cpp
                           ${SRC_DIR}/tests/run_tests.cpp)

file(GLOB ALL_TEST_HEADERS ${SRC_DIR}/tests/*.h
                           ${SRC_DIR}/tests/*.hpp)

add_executable(runTests ${ALL_TEST_SOURCES} ${ALL_TEST_HEADERS} ${ALL_SOURCES} ${ALL_HEADERS})

target_link_libraries(runTests ${GTEST_BOTH_LIBRARIES})
target_link_libraries(runTests ${GMock_BOTH_LIBRARIES})
target_link_libraries(runTests ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(runTests ${Boost_LIBRARIES})

file(GLOB SOURCES ${SRC_DIR}/core/*.cpp
                  ${SRC_DIR}/network/*.cpp
                  ${SRC_DIR}/filesystem/*.cpp)

file(GLOB HEADERS ${SRC_DIR}/core/*.h
                  ${SRC_DIR}/network/*.h
                  ${SRC_DIR}/file_system/*.h)

add_executable(failless-cli ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${ALL_SOURCES} ${ALL_HEADERS})

target_link_libraries(failless-cli ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(failless-cli ${Boost_LIBRARIES})
target_link_libraries(failless-cli ${CMAKE_THREAD_LIBS_INIT})

message("Boost version ${Boost_VERSION} found at ${Boost_DIR}")
