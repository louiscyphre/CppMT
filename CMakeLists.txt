project (CMT)

cmake_minimum_required (VERSION 2.6)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

option(BUILD_TRAX_CLIENT "Build the trax client." OFF)

find_package(OpenCV REQUIRED)

if (DEPENDENCY_PROJECT AND ROOT_INCLUDE_DIR)
    set(TARGET_INCLUDE_DIR "${ROOT_INCLUDE_DIR}")
elseif(NOT DEPENDENCY_PROJECT AND NOT ROOT_INCLUDE_DIR)
    set(TARGET_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}")
else()
    message(FATAL_ERROR "
        FATAL: Check ROOT_INCLUDE_DIR and DEPENDENCY_PROJECT defines.
                       They both must be or set or unset.
        ")
endif()

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include"
        DESTINATION "${TARGET_INCLUDE_DIR}"
        FILES_MATCHING PATTERN "*.h" )

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_definitions ("-Wall")

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC  -std=c++11")

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

set(LIBRARY_NAME  cmt)

add_library (${LIBRARY_NAME} SHARED common.cpp gui.cpp
    CMT.cpp Consensus.cpp Fusion.cpp Matcher.cpp Tracker.cpp
    fastcluster/fastcluster.cpp)

target_link_libraries(${LIBRARY_NAME} ${OpenCV_LIBS})

