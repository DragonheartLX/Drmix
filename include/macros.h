#pragma once
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.h>

#define VkCall(func, msg)                           \
    if (func != VK_SUCCESS)                         \
    {                                               \
        spdlog::error("VkCall error: {0}", msg);    \
        throw std::runtime_error("Vulkan error");   \
    }