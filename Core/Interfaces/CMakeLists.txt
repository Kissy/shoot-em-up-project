cmake_minimum_required(VERSION 2.8)

project(Interfaces)

find_package(Boost REQUIRED)
find_package(Protobuf REQUIRED)

include_directories(${BOOST_INCLUDE_DIRS})
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(Source)
include_directories(${BaseTypes_SOURCE_DIR}/Source)
include_directories(${Generated_SOURCE_DIR}/Source)

aux_source_directory(Source/Generic Interfaces_Generic_SOURCES)
source_group(Source\\Generic FILES ${Interfaces_Generic_SOURCES})

aux_source_directory(Source/Manager Interfaces_Manager_SOURCES)
source_group(Source\\Manager FILES ${Interfaces_Manager_SOURCES})

aux_source_directory(Source/Object Interfaces_Object_SOURCES)
source_group(Source\\Object FILES ${Interfaces_Object_SOURCES})

aux_source_directory(Source/System Interfaces_System_SOURCES)
source_group(Source\\System FILES ${Interfaces_System_SOURCES})

add_library(Interfaces STATIC ${Interfaces_Generic_SOURCES} ${Interfaces_Manager_SOURCES} 
    ${Interfaces_Object_SOURCES} ${Interfaces_System_SOURCES})

add_dependencies(Interfaces BaseTypes Generated)
