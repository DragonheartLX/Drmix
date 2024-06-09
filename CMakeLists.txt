cmake_minimum_required (VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

# MSVC设置: 以utf8编译
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/lib>)
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")

project(Drmix)

# MSVC设置: 静态库设置
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(
        "$<$<CONFIG:>:/MT>" #---------|
        "$<$<CONFIG:Debug>:/MTd>" #---|-- Statically link the runtime libraries
        "$<$<CONFIG:Release>:/MT>" #--|
    )
endif()

file(GLOB_RECURSE PROJ_Src LIST_DIRECTORIES false 
    src/*.cpp
    src/**/*.cpp)

add_executable(Drmix ${PROJ_Src})

target_include_directories(Drmix PRIVATE include)

find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(Drmix PRIVATE glfw)

find_package(Vulkan REQUIRED)
    target_link_libraries(Drmix PRIVATE Vulkan::Vulkan)

find_package(fmt CONFIG REQUIRED)
    target_link_libraries(Drmix PRIVATE fmt::fmt)

find_package(PkgConfig REQUIRED)
    pkg_check_modules(tomlplusplus REQUIRED IMPORTED_TARGET tomlplusplus)
    target_link_libraries(Drmix PRIVATE PkgConfig::tomlplusplus)

find_program(GLSLC_PROGRAM glslc REQUIRED)
    execute_process(COMMAND ${GLSLC_PROGRAM} ${CMAKE_SOURCE_DIR}/res/shader.vert -o ${CMAKE_SOURCE_DIR}/cache/vert.spv)
    execute_process(COMMAND ${GLSLC_PROGRAM} ${CMAKE_SOURCE_DIR}/res/shader.frag -o ${CMAKE_SOURCE_DIR}/cache/frag.spv)