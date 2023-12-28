# ShaderToyMixture CMake file
file(GLOB_RECURSE Mixture_Src LIST_DIRECTORIES false 
    *.cpp
    **/*.cpp)

add_executable(ShaderToyMixture ${Mixture_Src})

target_include_directories(ShaderToyMixture PRIVATE .)

target_link_libraries(ShaderToyMixture PRIVATE DrmixCore)

find_package(imgui CONFIG REQUIRED)
    target_link_libraries(ShaderToyMixture PRIVATE imgui::imgui)

find_package(glm CONFIG REQUIRED)
    target_link_libraries(ShaderToyMixture PRIVATE glm::glm)

install(TARGETS ShaderToyMixture DESTINATION bin)