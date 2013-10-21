cmake_minimum_required(VERSION 2.8)

project(BaseTypes)

aux_source_directory(Source BaseTypes_SOURCES)

add_library(BaseTypes STATIC ${BaseTypes_SOURCES})

# Testing
if(BUILD_TEST)
    find_package(GTest REQUIRED)
    
    include_directories(${GTEST_INCLUDE_DIRS})
    aux_source_directory(Test BaseTypesTest_SOURCES)

    add_executable(BaseTypesTest ${BaseTypesTest_SOURCES})
    target_link_libraries(BaseTypesTest BaseTypes pthread ${GTEST_BOTH_LIBRARIES})

    add_test(SingletonTest BaseTypesTest)
endif()