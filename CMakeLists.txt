cmake_minimum_required(VERSION 3.15)
project(failless C CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost ${BOOST_VERSION} COMPONENTS serialization filesystem chrono thread system REQUIRED)
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/llssclient")

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(msgpack REQUIRED)

find_library(ROCKSDB_LIBRARY rocksdb
  HINTS ${CMAKE_SOURCE_DIR}/../../.. $ENV{ROCKSDB_ROOT} ${ROCKSDB_ROOT}
  PATH_SUFFIXES lib lib64
  PATHS /usr /usr/local /opt /opt/local)

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

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -std=c++17 -fPIC -no-pie -phtread -lpthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -std=c++17 -fPIC -no-pie -pthread -lpthread")

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMock_INCLUDE_DIR})

include(${CMAKE_CURRENT_SOURCE_DIR}/client/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/db/CMakeLists.txt)

message("Boost version ${Boost_VERSION} found at ${Boost_DIR}")
message("Msgpack version ${msgpack_VERSION} found at ${msgpack_DIR}")
message("RocksDB library at ${ROCKSDB_LIBRARY}")
