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

add_subdirectory(Drmix)
add_subdirectory(Mixture)

