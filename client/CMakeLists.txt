set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/llssclient/)

include_directories(${SRC_DIR})

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

add_executable(run-client-tests ${TEST_SOURCES} ${TEST_HEADERS} ${SOURCES} ${HEADERS})
set_target_properties(run-client-tests PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(run-client-tests ${GTEST_BOTH_LIBRARIES})
target_link_libraries(run-client-tests ${GMock_BOTH_LIBRARIES})
target_link_libraries(run-client-tests ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(run-client-tests ${Boost_LIBRARIES})
target_link_libraries(run-client-tests ${msgpack_LIBRARIES})

add_executable(failless-cli ${CMAKE_CURRENT_SOURCE_DIR}/client/main.cpp ${SOURCES} ${HEADERS})

target_link_libraries(failless-cli ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(failless-cli ${Boost_LIBRARIES})
target_link_libraries(failless-cli ${msgpack_LIBRARIES})
