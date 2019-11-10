cmake_minimum_required(VERSION 3.15)
project(llssclient C CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost ${BOOST_VERSION} COMPONENTS serialization filesystem thread system REQUIRED)
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/llssclient")

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(msgpack REQUIRED)

set(GMock_INCLUDE_DIR "/usr/local/include")
if(EXISTS "/usr/local/lib/libgmock.a")
    set(GMock_LIBRARY "/usr/local/lib/libgmock.a")
    set(GMock_LIBRARY_MAIN "/usr/local/lib/libgmock_main.a")
    message("Gmock is a static build")
else()
    set(GMock_LIBRARY "/usr/local/lib/libgmock.so")
    set(GMock_LIBRARY "/usr/local/lib/libgmock.so")
    message("Gmock is a dynamic build")
endif()
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

file(GLOB SOURCES ${SRC_DIR}/core/*.cpp
                  ${SRC_DIR}/network/*.cpp
                  ${SRC_DIR}/filesystem/*.cpp)

file(GLOB HEADERS ${SRC_DIR}/core/*.h
                  ${SRC_DIR}/network/*.h
                  ${SRC_DIR}/file_system/*.h)

file(GLOB TEST_SOURCES ${SRC_DIR}/tests/test_client.cpp
                       ${SRC_DIR}/tests/run_tests.cpp)

file(GLOB TEST_HEADERS ${SRC_DIR}/tests/*.h
                       ${SRC_DIR}/tests/*.hpp)

add_executable(runTests ${TEST_SOURCES} ${TEST_HEADERS} ${SOURCES} ${HEADERS})

target_link_libraries(runTests ${GTEST_BOTH_LIBRARIES})
target_link_libraries(runTests ${GMock_BOTH_LIBRARIES})
target_link_libraries(runTests ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(runTests ${Boost_LIBRARIES})
target_link_libraries(runTests ${msgpack_LIBRARIES})

add_executable(failless-cli ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${SOURCES} ${HEADERS})

target_link_libraries(failless-cli ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(failless-cli ${Boost_LIBRARIES})
target_link_libraries(failless-cli ${msgpack_LIBRARIES})

message("Boost version ${Boost_VERSION} found at ${Boost_DIR}")
message("Msgpack version ${msgpack_VERSION} found at ${msgpack_DIR}")
