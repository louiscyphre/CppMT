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

#file(GLOB MY_PUBLIC_HEADERS
#  "./*.h"
#  "fastcluster/fastcluster.h"
#  "logging/log.h"
#  "getopt/getopt.h"
#)
#file(COPY ${MY_PUBLIC_HEADERS} DESTINATION myDestination)

set(TARGET_INCLUDE_DIR "../../lib/cppmt")

install(DIRECTORY "./include"
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
    fastcluster/fastcluster.cpp getopt/getopt.cpp
    )

target_link_libraries(${LIBRARY_NAME} ${OpenCV_LIBS})

if(BUILD_TRAX_CLIENT)
    set(TRAX_DIR "" CACHE FILEPATH "Path to trax")

    include_directories(${TRAX_DIR}/include)

    add_executable (trax_client cmt.so common.cpp gui.cpp trax.cpp
        CMT.cpp Consensus.cpp Fusion.cpp Matcher.cpp Tracker.cpp
        fastcluster/fastcluster.cpp)

    find_library(TRAX_LIBRARY NAMES trax HINTS "${TRAX_DIR}/lib")

    target_link_libraries(trax_client ${OpenCV_LIBS} ${TRAX_LIBRARY})

endif()

