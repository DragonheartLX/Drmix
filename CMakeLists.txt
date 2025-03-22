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

find_package(glm CONFIG REQUIRED)
    target_link_libraries(Drmix PRIVATE glm::glm)

find_package(Vulkan REQUIRED)
    target_link_libraries(Drmix PRIVATE Vulkan::Vulkan)

find_package(spdlog REQUIRED)
    target_link_libraries(Drmix PRIVATE spdlog::spdlog)

# if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
#     string(REPLACE "/" "\\\\" VK_LAYER "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/bin")
#     add_compile_definitions(VK_VALIDATIONLAYERS="${VK_LAYER}")
# else()
#     string(REPLACE "/" "\\\\" VK_LAYER "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin")
#     add_compile_definitions(VK_VALIDATIONLAYERS="${VK_LAYER}")
# endif()
