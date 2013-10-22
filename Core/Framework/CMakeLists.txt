cmake_minimum_required(VERSION 2.8)

project(Framework)

find_package(Boost REQUIRED)
find_package(Protobuf REQUIRED)
find_package(ZeroMQ REQUIRED)
find_package(TBB REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${ZEROMQ_INCLUDE_DIRS})
include_directories(${TBB_INCLUDE_DIRS})

include_directories(Source)
include_directories(${BaseTypes_SOURCE_DIR}/Source)
include_directories(${Generated_SOURCE_DIR}/Source)
include_directories(${Interfaces_SOURCE_DIR}/Source)

aux_source_directory(Source Interfaces_SOURCES)
source_group(Source FILES ${Interfaces_SOURCES})

aux_source_directory(Source/Debugger Interfaces_Debugger_SOURCES)
source_group(Source\\Debugger FILES ${Interfaces_Debugger_SOURCES})

aux_source_directory(Source/Generic Interfaces_Generic_SOURCES)
source_group(Source\\Generic FILES ${Interfaces_Generic_SOURCES})

aux_source_directory(Source/Manager Interfaces_Manager_SOURCES)
source_group(Source\\Manager FILES ${Interfaces_Manager_SOURCES})

aux_source_directory(Source/Service Interfaces_Service_SOURCES)
source_group(Source\\Service FILES ${Interfaces_Service_SOURCES})

aux_source_directory(Source/Task Interfaces_Task_SOURCES)
source_group(Source\\Task FILES ${Interfaces_Task_SOURCES})

aux_source_directory(Source/Universal Interfaces_Universal_SOURCES)
source_group(Source\\Universal FILES ${Interfaces_Universal_SOURCES})

add_library(Framework STATIC ${Interfaces_SOURCES} ${Interfaces_Debugger_SOURCES} 
    ${Interfaces_Generic_SOURCES} ${Interfaces_Manager_SOURCES}
    ${Interfaces_Service_SOURCES} ${Interfaces_Task_SOURCES}
    ${Interfaces_Universal_SOURCES})

add_dependencies(Framework BaseTypes Generated Interfaces)
