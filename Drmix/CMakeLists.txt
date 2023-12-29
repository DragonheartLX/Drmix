# Drmix core
file(GLOB_RECURSE DrmixCore_SRC LIST_DIRECTORIES false 
    *.cpp
    **/*.cpp)

project(DrmixCore)

add_library(DrmixCore STATIC ${DrmixCore_SRC})

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set_property(TARGET DrmixCore APPEND PROPERTY LINK_FLAGS "/NODEFAULTLIB:MSVCRT")
endif()

target_include_directories(DrmixCore PUBLIC .)

find_package(glad CONFIG REQUIRED)
    target_link_libraries(DrmixCore PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(DrmixCore PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
    target_link_libraries(DrmixCore PUBLIC glm::glm)

find_package(spdlog CONFIG REQUIRED)
    target_link_libraries(DrmixCore PUBLIC spdlog::spdlog_header_only)

find_package(imgui CONFIG REQUIRED)
    target_link_libraries(DrmixCore PUBLIC imgui::imgui)

find_package(Stb REQUIRED)
    target_include_directories(DrmixCore PRIVATE ${Stb_INCLUDE_DIR})

install(TARGETS DrmixCore DESTINATION lib)