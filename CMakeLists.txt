cmake_minimum_required(VERSION 2.8)

project(ShootEmUpProject)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-pragmas -std=c++11")
endif()

ADD_SUBDIRECTORY(Core)
