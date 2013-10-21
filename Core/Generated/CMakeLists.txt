cmake_minimum_required(VERSION 2.8)

project(Generated)

include_directories(Source)

aux_source_directory(Source/Proto Generated_Proto_SOURCES)
source_group(Source\\Proto FILES ${Generated_Proto_SOURCES})

aux_source_directory(Source/Proto/Server Generated_Proto_Server_SOURCES)
source_group(Source\\Proto\\Server FILES ${Generated_Proto_Server_SOURCES})

add_library(Generated STATIC ${Generated_Proto_SOURCES} ${Generated_Proto_Server_SOURCES})
