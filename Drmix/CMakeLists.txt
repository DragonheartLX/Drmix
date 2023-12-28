# Drmix core
file(GLOB_RECURSE DrmixCore_SRC LIST_DIRECTORIES false 
    *.cpp
    **/*.cpp)

project(DrmixCore)

add_library(DrmixCore STATIC ${DrmixCore_SRC})

target_include_directories(DrmixCore PUBLIC .)

find_package(glad CONFIG REQUIRED)
    target_link_libraries(DrmixCore PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(DrmixCore PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
    target_link_libraries(DrmixCore PRIVATE glm::glm)

find_package(spdlog CONFIG REQUIRED)
    target_link_libraries(DrmixCore PRIVATE spdlog::spdlog_header_only)

find_package(imgui CONFIG REQUIRED)
    target_link_libraries(DrmixCore PRIVATE imgui::imgui)

find_package(Stb REQUIRED)
    target_include_directories(DrmixCore PRIVATE ${Stb_INCLUDE_DIR})

install(TARGETS DrmixCore DESTINATION lib)