cmake_minimum_required(VERSION 3.20)

project(DrmixProject VERSION 1.0)

find_package(Qt6 REQUIRED COMPONENTS Core)

# if(CMAKE_BUILD_TYPE MATCHES "Debug")
#     set(QT_SDK_DIR "${Qt6_DIR}/../../debug/Qt6/")
# else()
#     set(QT_SDK_DIR "${Qt6_DIR}/../../Qt6/")
# endif()
# cmake_path(SET QT_SDK_DIR NORMALIZE ${QT_SDK_DIR})

# set(FLUENTUI_BUILD_STATIC_LIB ON)

add_subdirectory(FluentUI)
add_subdirectory(Drmix)
