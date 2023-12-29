# ShaderToyMixture CMake file
file(GLOB_RECURSE Mixture_Src LIST_DIRECTORIES false 
    *.cpp
    **/*.cpp)

add_executable(ShaderToyMixture ${Mixture_Src})

target_include_directories(ShaderToyMixture PRIVATE .)

target_link_libraries(ShaderToyMixture PRIVATE DrmixCore)

install(TARGETS ShaderToyMixture DESTINATION bin)