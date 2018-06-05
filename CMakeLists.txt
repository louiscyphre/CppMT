project (CMT)

cmake_minimum_required (VERSION 2.6)

find_package(OpenCV REQUIRED)

add_definitions ("-Wall")
add_definitions("-Wall")
add_definitions("-Wextra")
add_definitions("-pedantic")

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -Wextra -pedantic")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")

add_executable (cmt common.cpp gui.cpp main.cpp
    CMT.cpp Consensus.cpp Fusion.cpp Matcher.cpp Tracker.cpp
    fastcluster/fastcluster.cpp)

target_link_libraries(cmt ${OpenCV_LIBS})
