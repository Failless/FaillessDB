cmake_minimum_required(VERSION 3.14)
project(failless C CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(Boost ${BOOST_VERSION} COMPONENTS serialization filesystem chrono thread system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -std=c++17 -fPIC -no-pie -phtread -lpthread -lboost_system")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -std=c++17 -fPIC -no-pie -pthread -lpthread -lboost_system")

set(DATABSE_SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/failless_db.cpp)
set(CLIENT_SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/failless_cli.cpp)

set(FAILLESS_CLIENT_LIB "${CMAKE_CURRENT_SOURCE_DIR}/llsscli")
set(FAILLESS_DB_LIB "${CMAKE_CURRENT_SOURCE_DIR}/llssdb")

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

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMock_INCLUDE_DIR})

add_subdirectory(${FAILLESS_DB_LIB})
add_subdirectory(${FAILLESS_CLIENT_LIB})

# compile server
add_executable(failless-server ${DATABSE_SRC_LIST})
target_link_libraries(failless-server llssdb)

# compile client
add_executable(failless-cli ${CLIENT_SRC_LIST})
target_link_libraries(failless-cli llsscli)

set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

######llssdb test sources#######
file(GLOB TEST_DB_SOURCES ${TESTS_DIR}/tests_db/*.cpp
        ${TESTS_DIR}/tests_db/simple/*.cpp
        ${TESTS_DIR}/tests_db/run_tests.cpp)

file(GLOB TEST_DB_HEADERS ${TESTS_DIR}/tests_db/*.h
        ${TESTS_DIR}/tests_db/simple/*.hpp
        ${TESTS_DIR}/tests_db/simple/*.h
        ${TESTS_DIR}/tests_db/*.hpp)

######llsscli test sources########
file(GLOB TEST_CLI_SOURCES ${TESTS_DIR}/tests_cli/test_client.cpp
        ${TESTS_DIR}/tests_cli/run_tests.cpp)

file(GLOB TEST_CLI_HEADERS ${TESTS_DIR}/tests_cli/*.h
        ${TESTS_DIR}/tests_cli/*.hpp)


add_executable(run-cli-tests ${TEST_CLI_SOURCES} ${TEST_CLI_HEADERS})
target_link_libraries(run-cli-tests ${GTEST_BOTH_LIBRARIES})
target_link_libraries(run-cli-tests ${GMock_BOTH_LIBRARIES})
target_link_libraries(run-cli-tests ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(run-cli-tests ${Boost_LIBRARIES})
target_link_libraries(run-cli-tests ${msgpack_LIBRARIES})
target_link_libraries(run-cli-tests llsscli)

add_executable(run-db-tests ${TEST_DB_SOURCES} ${TEST_DB_HEADERS})
target_link_libraries(run-db-tests ${GTEST_BOTH_LIBRARIES})
target_link_libraries(run-db-tests ${GMock_BOTH_LIBRARIES})
target_link_libraries(run-db-tests ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(run-db-tests llssdb)
target_link_libraries(run-db-tests ${Boost_LIBRARIES})
target_link_libraries(run-db-tests ${msgpack_LIBRARIES})

message("Boost version ${Boost_VERSION} found at ${Boost_DIR}")
message("Msgpack version ${msgpack_VERSION} found at ${msgpack_DIR}")