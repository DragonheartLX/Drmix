# Drmix CMake file
file(GLOB_RECURSE Drmix_Src LIST_DIRECTORIES false 
    *.cpp
    **/*.cpp)

add_executable(DrmixMain ${Drmix_Src})

target_include_directories(DrmixMain PRIVATE .)

find_package(glad CONFIG REQUIRED)
    target_link_libraries(DrmixMain PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(DrmixMain PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
    target_link_libraries(DrmixMain PRIVATE glm::glm)

find_package(spdlog CONFIG REQUIRED)
    target_link_libraries(DrmixMain PRIVATE spdlog::spdlog_header_only)

find_package(Stb REQUIRED)
    target_include_directories(DrmixMain PRIVATE ${Stb_INCLUDE_DIR})

find_package(imgui CONFIG REQUIRED)
  target_link_libraries(DrmixMain PRIVATE imgui::imgui)

install(TARGETS DrmixMain DESTINATION bin)