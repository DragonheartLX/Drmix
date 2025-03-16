cmake_minimum_required (VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib>)
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")

project(Drmix)

file(GLOB_RECURSE PROJ_Src LIST_DIRECTORIES false 
    src/*.cpp
    src/**/*.cpp)

add_executable(Drmix ${PROJ_Src})

target_include_directories(Drmix PRIVATE include)

find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(Drmix PRIVATE glfw)