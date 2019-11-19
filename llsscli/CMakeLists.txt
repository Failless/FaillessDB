project(llsscli CXX)
set(CMAKE_CXX_STANDARD 17)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${SRC_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SOURCES ${SRC_DIR}/core/*.cpp
                  ${SRC_DIR}/network/*.cpp
                  ${SRC_DIR}/filesystem/*.cpp)

file(GLOB HEADERS ${SRC_DIR}/core/*.h
                  ${SRC_DIR}/network/*.h
                  ${SRC_DIR}/file_system/*.h)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

