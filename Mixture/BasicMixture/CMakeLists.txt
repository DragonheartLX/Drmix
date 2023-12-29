# Mixture CMake file
file(GLOB_RECURSE Mixture_Src LIST_DIRECTORIES false 
    *.cpp
    **/*.cpp)

add_executable(BasicMixture ${Mixture_Src})

target_include_directories(BasicMixture PRIVATE .)

target_link_libraries(BasicMixture PRIVATE DrmixCore)

install(TARGETS BasicMixture DESTINATION bin)