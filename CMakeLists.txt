cmake_minimum_required(VERSION 3.20)

project(DrmixProject VERSION 1.0)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_subdirectory(FluentUI)
add_subdirectory(Drmix)
