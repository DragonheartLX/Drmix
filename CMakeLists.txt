cmake_minimum_required (VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

# MSVC设置: 以utf8编译
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

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
    target_link_libraries(Drmix PRIVATE fmt::fmt-header-only) 