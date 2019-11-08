cmake_minimum_required(VERSION 3.15)
project(Failless)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Failless SHARED
        main.cpp
        Registration.cpp Registration.h
        SignIn.cpp SignIn.h
        Disconnector.cpp Disconnector.h
        Authorization.cpp Authorization.h)

# Set GTEST_INCLUDE_DIRS and GTEST_LIBRARIES
find_package(GTest REQUIRED)
# Add GoogleTest include directory
include_directories(${GTEST_INCLUDE_DIRS})

# Testing
add_executable(main.out main.cpp)
# Link GoogleTest
target_link_libraries(main.out ${GTEST_LIBRARIES})
target_link_libraries(main.out pthread)


set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.71.0 COMPONENTS *boost libraries here*)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(library library.cpp)
    target_link_libraries(library ${Boost_LIBRARIES})

endif()
