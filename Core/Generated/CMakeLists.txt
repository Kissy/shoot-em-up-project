cmake_minimum_required(VERSION 2.8)

project("Generated")

check_root_module()
add_project_sources("Source")
add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES})
