cmake_minimum_required(VERSION 2.8)

project("GraphicSystem")

check_root_module()

find_package("Ogre" COMPONENTS "Overlay" REQUIRED)
find_package("Berkelium" REQUIRED)
include_directories(${OGRE_INCLUDE_DIRS} ${BERKELIUM_INCLUDE_DIRS})

add_project_dependency("Framework")
add_project_sources("Source")
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES}
    "${PROJECT_NAME}.def")
add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES})
target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES} 
    ${PROTOBUF_LIBRARIES} ${LOGOG_LIBRARIES}
    ${OGRE_LIBRARIES} ${BERKELIUM_LIBRARIES})
