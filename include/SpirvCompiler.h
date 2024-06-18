#pragma once

#include <cstdint>
#include <glslang/Include/glslang_c_shader_types.h>

struct SpirVBinary
{
    uint32_t* code;
    int size;
};

SpirVBinary compileShader(glslang_stage_t stage, const char* shaderSource, const char* fileName);