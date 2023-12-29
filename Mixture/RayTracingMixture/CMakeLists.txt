# Mixture CMake file
file(GLOB_RECURSE Mixture_Src LIST_DIRECTORIES false 
    *.cpp
    **/*.cpp)

add_executable(RayTracingMixture ${Mixture_Src})

target_include_directories(RayTracingMixture PRIVATE .)

target_link_libraries(RayTracingMixture PRIVATE DrmixCore)

install(TARGETS RayTracingMixture DESTINATION bin)