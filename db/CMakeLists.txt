set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/llssdb/)
include_directories(${SRC_DIR})

file(GLOB SOURCES ${SRC_DIR}/lua/*.cpp
        ${SRC_DIR}/folder/*.cpp
        ${SRC_DIR}/engine/*.cpp
        ${SRC_DIR}/network/*.cpp
        ${SRC_DIR}/network/transfer/*.cpp
        ${SRC_DIR}/filesystem/*.cpp)

file(GLOB HEADERS ${SRC_DIR}/lua/*.h
        ${SRC_DIR}/folder/*.h
        ${SRC_DIR}/engine/*.h
        ${SRC_DIR}/network/*.h
        ${SRC_DIR}/network/transfer/*.h
        ${SRC_DIR}/filesystem/*.h)

file(GLOB TEST_SOURCES ${SRC_DIR}/tests/test_client.cpp
        ${SRC_DIR}/tests/simple/*.cpp
        ${SRC_DIR}/tests/run_tests.cpp)

file(GLOB TEST_HEADERS ${SRC_DIR}/tests/*.h
        ${SRC_DIR}/tests/simple/*.hpp
        ${SRC_DIR}/tests/simple/*.h
        ${SRC_DIR}/tests/*.hpp)


add_executable(run-db-tests ${TEST_SOURCES} ${TEST_HEADERS} ${SOURCES} ${HEADERS})
set_target_properties(run-db-tests PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(run-db-tests ${GTEST_BOTH_LIBRARIES})
target_link_libraries(run-db-tests ${GMock_BOTH_LIBRARIES})
target_link_libraries(run-db-tests ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(run-db-tests ${Boost_LIBRARIES})
target_link_libraries(run-db-tests ${msgpack_LIBRARIES})

add_executable(failless-server ${CMAKE_CURRENT_SOURCE_DIR}/db/main.cpp ${SOURCES} ${HEADERS})

target_link_libraries(failless-server ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(failless-server ${Boost_LIBRARIES})
target_link_libraries(failless-server ${msgpack_LIBRARIES})
