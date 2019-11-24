project(llssdb)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${SRC_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SOURCES ${SRC_DIR}/lua/*.cpp
        ${SRC_DIR}/folder/*.cpp
        ${SRC_DIR}/auth/*.cpp
        ${SRC_DIR}/config/*.cpp
        ${SRC_DIR}/common/*.cpp
        ${SRC_DIR}/utils/*.cpp
        ${SRC_DIR}/engine/*.cpp
        ${SRC_DIR}/network/*.cpp
        ${SRC_DIR}/network/transfer/*.cpp
        ${SRC_DIR}/filesystem/*.cpp)

file(GLOB HEADERS ${SRC_DIR}/lua/*.h
        ${SRC_DIR}/folder/*.h
        ${SRC_DIR}/auth/*.h
        ${SRC_DIR}/config/*.h
        ${SRC_DIR}/common/*.h
        ${SRC_DIR}/utils/*.h
        ${SRC_DIR}/engine/*.h
        ${SRC_DIR}/network/*.h
        ${SRC_DIR}/network/transfer/*.h
        ${SRC_DIR}/filesystem/*.h)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS} auth/user_info.h auth/iauthorization.h config/configmanager.h config/settings.h config/configmanager.cpp)
# target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
