cmake_minimum_required(VERSION 3.14)

project(failless C CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost ${BOOST_VERSION} COMPONENTS serialization log log_setup filesystem chrono thread system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -Werror -std=c++17 -DBOOST_LOG_DYN_LINK -pthread -lpthread -lbz2 -lz -lssl -lcrypto -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include -lboost_log-mt -lboost_thread-mt -lboost_system-mt")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -std=c++17 -DBOOST_LOG_DYN_LINK -pthread -lpthread -lbz2 -lz -lssl -lcrypto -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include -lboost_log-mt -lboost_thread-mt -lboost_system-mt")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -Werror -std=c++17 -DBOOST_LOG_DYN_LINK -pthread -lpthread -lbz2 -lz -lssl -lcrypto -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -std=c++17 -DBOOST_LOG_DYN_LINK -pthread -lpthread -lbz2 -lz -lssl -lcrypto -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include")
endif()

#remove -lrocksdb, -lsnappy and -lrt
#add -lboost_log-mt -lboost_thread-mt -lboost_system-mt if you want
#added -lssl -lcrypto for good work of OpenSSL hashing

set(Boost_LIBRARY_DIR /usr/local/lib)
link_directories(${Boost_LIBRARY_DIR})

set(OpenSSL_LIBRARY_DIR /usr/local/lib)
link_directories(${OpenSSL_LIBRARY_DIR})

set(DATABASE_SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/failless_db.cpp)
set(CLIENT_SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/failless_cli.cpp)

set(FAILLESS_CLI_LIB "${CMAKE_CURRENT_SOURCE_DIR}/llsscli")
set(FAILLESS_DB_LIB "${CMAKE_CURRENT_SOURCE_DIR}/llssdb")
set(FAILLESS_3P_LIB "${CMAKE_CURRENT_SOURCE_DIR}/llss3p")

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(msgpack REQUIRED)
find_library(ROCKSDB_LIBRARY rocksdb REQUIRED)

###### Finding OpenSSL ######
if (EXISTS "/usr/local/opt/openssl@1.1")
    # run next command if you're on macOS
    # echo 'export PATH="/usr/local/opt/openssl@1.1/bin:$PATH"' >> ~/.zshrc
    # and go here if sha.h wasn't found
    # https://github.com/mongodb/mongo-php-driver/issues/523
    set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl@1.1/lib")
    set(OPENSSL_INCLUDE_DIR "/usr/local/opt/openssl@1.1/include")
endif()
find_package(OpenSSL REQUIRED)


set(GMock_INCLUDE_DIR "/usr/local/include")
if (EXISTS "/usr/local/lib/libgmock.a")
    set(GMock_LIBRARY "/usr/local/lib/libgmock.a")
    set(GMock_LIBRARY_MAIN "/usr/local/lib/libgmock_main.a")
    message("Gmock is a static build")
else ()
    set(GMock_LIBRARY "/usr/local/lib/libgmock.so")
    set(GMock_LIBRARY "/usr/local/lib/libgmock.so")
    message("Gmock is a dynamic build")
endif ()
set(GMock_BOTH_LIBRARIES ${GMock_LIBRARY} ${GMock_LIBRARY_MAIN})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMock_INCLUDE_DIR})
include_directories(${ROCKSDB_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIRS})

add_subdirectory(${FAILLESS_DB_LIB})
add_subdirectory(${FAILLESS_CLI_LIB})
add_subdirectory(${FAILLESS_3P_LIB})

# compile server
add_executable(failless-server ${DATABASE_SRC_LIST})
target_link_libraries(failless-server llssdb)
target_link_libraries(failless-server llss3p)
target_link_libraries(failless-server ${Boost_LIBRARIES})
target_link_libraries(failless-server ${msgpack_LIBRARIES})
target_link_libraries(failless-server ${ROCKSDB_LIBRARY})
target_link_libraries(failless-server ${OPENSSL_LIBRARIES})

# compile client
add_executable(failless-cli ${CLIENT_SRC_LIST})
target_link_libraries(failless-cli llsscli)
target_link_libraries(failless-cli llss3p)
target_link_libraries(failless-cli ${Boost_LIBRARIES})
target_link_libraries(failless-cli ${msgpack_LIBRARIES})

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
target_link_libraries(run-cli-tests llss3p)
target_link_libraries(run-cli-tests ${ROCKSDB_LIBRARY})

add_executable(run-db-tests ${TEST_DB_SOURCES} ${TEST_DB_HEADERS})
target_link_libraries(run-db-tests ${GTEST_BOTH_LIBRARIES})
target_link_libraries(run-db-tests ${GMock_BOTH_LIBRARIES})
target_link_libraries(run-db-tests ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(run-db-tests llssdb)
target_link_libraries(run-db-tests llss3p)
target_link_libraries(run-db-tests ${Boost_LIBRARIES})
target_link_libraries(run-db-tests ${msgpack_LIBRARIES})
target_link_libraries(run-db-tests ${ROCKSDB_LIBRARY})
target_link_libraries(run-db-tests ${OPENSSL_LIBRARIES})


message("Boost version ${Boost_VERSION} found at ${Boost_LIBRARY_DIRS}")
message("Msgpack version ${msgpack_VERSION} found at ${msgpack_DIR}")
message("RocksDB version ${RocksDB_VERSION} found at ${RocksDB_DIR} and RocksDB library at ${ROCKSDB_LIBRARY}")
message("OpenSSL version ${OPENSSL_VERSION} found at ${OPENSSL_LIBRARIES}")
