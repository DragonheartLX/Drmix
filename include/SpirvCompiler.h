#pragma once

#include "Logger.h"
#include "Utility.h"

#include <cstdint>
#include <glslang/Public//resource_limits_c.h>
#include <glslang/Include/glslang_c_interface.h>
#include <glslang/Include/glslang_c_shader_types.h>
#include <vector>

struct SpirVBinary
{
    uint32_t* code;
    int size;
    uint32_t bytesCount = 0;

    static void initialize()
    {
        glslang_initialize_process();
    };

    static void finalize()
    {
        glslang_finalize_process();
    };

    SpirVBinary(glslang_stage_t stage, const char *fileName)
    {
        compileShader(stage, fileName);
    };

    void compileShader(glslang_stage_t stage, const char* fileName)
    {
        code = nullptr;
        size = 0;

        std::vector<char> shaderSource = utils::readFile(fileName);
        shaderSource.push_back('\0');

        const glslang_input_t input =
        {
            .language = GLSLANG_SOURCE_GLSL,
            .stage = stage,
            .client = GLSLANG_CLIENT_VULKAN,
            .client_version = GLSLANG_TARGET_VULKAN_1_0,
            .target_language = GLSLANG_TARGET_SPV,
            .target_language_version = GLSLANG_TARGET_SPV_1_3,
            .code = shaderSource.data(),
            .default_version = 100,
            .default_profile = GLSLANG_NO_PROFILE,
            .force_default_version_and_profile = false,
            .forward_compatible = false,
            .messages = GLSLANG_MSG_DEFAULT_BIT,
            .resource = glslang_default_resource(),
        };

        glslang_shader_t* shader = glslang_shader_create(&input);

        if (!glslang_shader_preprocess(shader, &input))
        {
            const char* msg[] =
            {
                fileName,
                glslang_shader_get_info_log(shader),
                glslang_shader_get_info_debug_log(shader)
            };

            Logger::error("GLSL preprocessing failed {0}", msg[0]);
            Logger::error("{0}", msg[1]);
            Logger::error("{0}", msg[2]);
            Logger::error("{0}", input.code);
            glslang_shader_delete(shader);
        }

        if (!glslang_shader_parse(shader, &input))
        {
            const char* msg[] =
            {
                fileName,
                glslang_shader_get_info_log(shader),
                glslang_shader_get_info_debug_log(shader),
                glslang_shader_get_preprocessed_code(shader)
            };

            Logger::error("GLSL parsing failed {0}", msg[0]);
            Logger::error("{0}", msg[1]);
            Logger::error("{0}", msg[2]);
            Logger::error("{0}", msg[3]);
            glslang_shader_delete(shader);
        }

        glslang_program_t* program = glslang_program_create();
        glslang_program_add_shader(program, shader);

        if (!glslang_program_link(program, GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT))
        {
            const char* msg[] =
            {
                fileName,
                glslang_program_get_info_log(program),
                glslang_program_get_info_debug_log(program)
            };

            Logger::error("GLSL linking failed {0}", msg[0]);
            Logger::error("{0}", msg[1]);
            Logger::error("{0}", msg[2]);
            glslang_program_delete(program);
            glslang_shader_delete(shader);
        }

        glslang_program_SPIRV_generate(program, stage);

        size = glslang_program_SPIRV_get_size(program);
        code = (uint32_t*)malloc(size * sizeof(uint32_t));
        glslang_program_SPIRV_get(program, code);

        const char* spirv_messages = glslang_program_SPIRV_get_messages(program);
        if (spirv_messages)
            Logger::info("({0}) {1}\b", fileName, spirv_messages);

        glslang_program_delete(program);
        glslang_shader_delete(shader);

        bytesCount = size * 4;
    }
};