project (CMT)

cmake_minimum_required (VERSION 3.1.3)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

set(CMAKE_COLOR_MAKEFILE   ON)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
    set(DEBUG TRUE)
    set(PROJECT_TARGET_FILES common.cpp
            CMT.cpp Fusion.cpp Matcher.cpp Tracker.cpp)# debug/gdb.c)
else()
    set(PROJECT_TARGET_FILES common.cpp
            CMT.cpp Fusion.cpp Matcher.cpp Tracker.cpp)
endif()

if (NOT DESCRIPTORS_TYPE)
    set(DESCRIPTORS_TYPE cv::ORB)
    #set(DESCRIPTORS_TYPE cv::FAST)
    #set(DESCRIPTORS_TYPE cv::xfeatures2d::SURF)
    set(DETECTOR_STR ${DESCRIPTORS_TYPE})
endif()

if (NOT MAX_POINTS)
    set(MAX_POINTS 500)
endif()

if (NOT POINTS_QUALITY_LEVEL_INIT)
    set(POINTS_QUALITY_LEVEL_INIT 0.3)
endif()

if (NOT POINTS_QUALITY_LEVEL_DETECT)
    set(POINTS_QUALITY_LEVEL_DETECT 0.3)
endif()

if (NOT MIN_DISTANCE)
    set(MIN_DISTANCE 5)
endif()

set(LIBRARY_NAME  cmt)


# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
        "${CMAKE_SOURCE_DIR}/include/cmt_config.h.in"
        "${CMAKE_BINARY_DIR}/include/cmt_config.h")

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR}/include)


add_library (${LIBRARY_NAME} SHARED ${PROJECT_TARGET_FILES})

target_compile_options(${LIBRARY_NAME} PUBLIC -Wall -Wextra -pedantic)
target_compile_options(${LIBRARY_NAME} PRIVATE -fPIC)

if(NOT DEPENDENCY_PROJECT)
    target_link_libraries(${LIBRARY_NAME} ${OpenCV_LIBS})
endif()

